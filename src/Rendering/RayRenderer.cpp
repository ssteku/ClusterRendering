#include "RayRenderer.hpp"
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreColourValue.h>
#include <OGRE/OgreRay.h>
#include <iostream>
#include <algorithm>
#include <OGRE/OgreMath.h>
#include <OGRE/OgrePlane.h>
using namespace std;

namespace rendering
{


RayRenderer::RayRenderer(boost::shared_ptr<Scene> scene) : scene(scene), MAX_DISTANCE(999999.0f), 
	NO_OBJECT(-1), NO_COLOR(0.0,0.0,0.0,1.0),START_LEVEL(0),START_COEF(1.0),ANTYALIASING_FACTOR(4)
{

}
void RayRenderer::render(std::vector < float > *pixels)
{
	cout<<"Antyaliasing: "<<scene->antyAliasing<<endl;

	// pixels->resize(3*(scene->window[1][1]-scene->window[0][1])*(scene->window[1][0]-scene->window[0][0]));
	float  x, y;  
	float x_fl, y_fl;    // pozycja rysowanego piksela "zmiennoprzecinkowa"
	float x_flTmp, y_flTmp;
	float x_fbase, y_fbase; //
	float im_size_x_2;       // po�owa rozmiaru obrazu w pikselach
	float im_size_y_2;
	Ogre::Vector3 starting_point;
	Ogre::Vector3 starting_direction(-scene->cameraZ[0],-scene->cameraZ[1],-scene->cameraZ[2]); // wektor opisuj�cy kierunek promienia
    im_size_x_2 = static_cast<float>(scene->dimension[0])/2;    // obliczenie po�owy rozmiaru obrazu w pikselach
    im_size_y_2 = static_cast<float>(scene->dimension[1])/2;

    x_fbase = static_cast<float>(scene->dimension[0])/static_cast<float>(scene->viewportSize[0]);
    y_fbase = static_cast<float>(scene->dimension[1])/static_cast<float>(scene->viewportSize[1]);
    Ogre::Plane plane(scene->cameraX,scene->cameraY,scene->cameraZ);
    float dist = plane.getDistance(scene->cameraPosition);

    Ogre::ColourValue pixel(0.0, 0.0, 0.0,1.0);
    Ogre::ColourValue finalPixel(0.0, 0.0, 0.0,1.0);
    float d = MAX_DISTANCE;

	Ogre::Ray ray(starting_point,starting_direction);
	const int maxBounceRate = scene->maxRayBounce;
	int currenObjId = NO_OBJECT;
	int currentLevel=START_LEVEL;
	float coef = START_COEF;


	Ogre::Vector3 normalVec;
	const float sampleRatio=1/ANTYALIASING_FACTOR;
	const float sampleDiff = 1/Ogre::Math::Sqrt(ANTYALIASING_FACTOR);
	// cout<<"Sample: "<<sampleDiff<<" sqrt: "<<Ogre::Math::Sqrt(ANTYALIASING_FACTOR)<<endl;
    for(y = im_size_y_2 - float(scene->window[0][1]); y > im_size_y_2 - float(scene->window[1][1]); --y) {
    	for(x = -im_size_x_2 + float(scene->window[0][0]); x < -im_size_x_2 + float(scene->window[1][0]); ++x) {
           
            finalPixel = NO_COLOR;
           
    		for (float fragmentx = static_cast<float>(x) ; fragmentx < x + 1.0f; fragmentx += sampleDiff )
		    for (float fragmenty = static_cast<float>(y) ; fragmenty < y + 1.0f; fragmenty += sampleDiff )
		    {
	            coef = START_COEF;
				currentLevel = START_LEVEL;

	            x_flTmp = (float)fragmentx/x_fbase;
	            y_flTmp = (float)fragmenty/y_fbase;


	            pixel = NO_COLOR;

	    		x_fl = x_flTmp;
	    		y_fl = y_flTmp;
	    		starting_point[0] = scene->cameraPosition[0] + (x_fl*scene->cameraX[0] + y_fl*scene->cameraY[0]) - dist*scene->cameraZ[0];
				starting_point[1] = scene->cameraPosition[1] + (x_fl*scene->cameraX[1] + y_fl*scene->cameraY[1]) - dist*scene->cameraZ[1];
				starting_point[2] = scene->cameraPosition[2] + (x_fl*scene->cameraX[2] + y_fl*scene->cameraY[2]) - dist*scene->cameraZ[2];// transformacja wsp kamery na globalne
				// cout<<"Start: "<<starting_point[0]<<" ,"<<starting_point[1]<<" ,"<<starting_point[2]<<endl;
				

				ray.setOrigin(scene->cameraPosition);

				ray.setDirection(starting_point - scene->cameraPosition);
				do
				{
					currenObjId = NO_OBJECT;
		            d = MAX_DISTANCE;

					for(unsigned int g=0;g< scene->objects.size();g++)
					{
						std::pair<bool,float> isInt = scene->objects[g]->intersects(ray);
						if(isInt.first && isInt.second < d  )
						{
							Ogre::Vector3 nVec = scene->objects[g]->getNormalVector(ray.getPoint(d));
							Ogre::Radian nRad = nVec.angleBetween(ray.getPoint(d));
							Ogre::Radian ang(Ogre::Degree(180.0));
							
							// if( nRad < ang)
							// {
								d = isInt.second;
								currenObjId = g;
							// }
						}
					}
					for(unsigned int g=0;g< scene->lights.size();g++)
					{
						std::pair<bool,float> isInt = scene->lights[g]->intersects(ray);
						if(isInt.first && isInt.second < d)
						{
							pixel = scene->lights[g]->getAmbient();
		                    break;
						}
					}

			
		

					if(currenObjId == -1)
					{
						pixel += scene->background;
						break;					
					}
				

			        Ogre::Vector3 newStart = ray.getPoint(d);
					normalVec = scene->objects[currenObjId]->getNormalVector(newStart);
					normalVec.normalise();
			        

		           	if(currentLevel==0)
		           	{
	                	pixel += scene->objects[currenObjId]->getAmbient()* scene->globalLight;
	                }
					for(unsigned int m=0;m<scene->lights.size();++m)
					{
						boost::shared_ptr<Light> current = scene->lights[m];
		          
		                Ogre::Vector3 lightVec = current->getSphere().getCenter()-newStart;
		                lightVec.normalise();
		                Ogre::Ray lightRay(newStart , lightVec);

		                bool inShadow = false; 
		                for (unsigned int h = 0; h < scene->objects.size(); ++h) {
		                    std::pair<bool,float> isInt = scene->objects[h]->intersects(lightRay);
							if(isInt.first )
							{
								inShadow = true;
		                        break;
							}
		                }
		                if (!inShadow) {
		                	
							
							countPhong(pixel,lightRay,ray,normalVec,current.operator*(),currenObjId,coef);							
							countBlinn(pixel,lightRay,ray,normalVec,current.operator*(),currenObjId,coef);
							pixel += current->getAmbient() * scene->objects[currenObjId]->getAmbient() ;
							coef *= scene->objects[currenObjId]->getN();
		                }
		          	                
					}
					
					 // We iterate on the next reflection
		            
		            float rayReflect = 2.0f * (ray.getDirection().dotProduct(normalVec));
		            ray.setOrigin(newStart);
		            ray.setDirection(ray.getDirection() - ((rayReflect) * normalVec));

					currentLevel++;

				}while((coef > 0.001f) && currentLevel<maxBounceRate);
				finalPixel += pixel*sampleRatio;
			}
            pixels->push_back(finalPixel.r);
            pixels->push_back(finalPixel.g);
            pixels->push_back(finalPixel.b);
    	}
    }

}

void RayRenderer::countPhong(Ogre::ColourValue& pixel, const Ogre::Ray& lightRay, 
		const Ogre::Ray& ray, const Ogre::Vector3& normalVec, Light& current, const int currenObjId, float coef)
{
	float lighRayReflect = 2*lightRay.getDirection().dotProduct(normalVec);
	Ogre::Vector3 phongDir = lightRay.getDirection() - (lighRayReflect*normalVec);
	float phongTerm = max(phongDir.dotProduct(ray.getDirection()),0.0f);
	phongTerm = scene->objects[currenObjId]->getN()*Ogre::Math::Pow(phongTerm, scene->objects[currenObjId]->getPhongN())*coef;
    pixel += phongTerm * current.getDiffuse()* scene->objects[currenObjId]->getDiffuse();
}

void RayRenderer::countBlinn(Ogre::ColourValue& pixel, const Ogre::Ray& lightRay, 
		const Ogre::Ray& ray, const Ogre::Vector3& normalVec, Light& current, const int currenObjId, float coef)
{
	Ogre::Vector3 blinnDir = lightRay.getDirection() - ray.getDirection();	
	float temp = blinnDir.length();
	if (temp != 0.0f )
	{
		blinnDir = (1.0f / temp)* blinnDir;
		float blinn =   max(blinnDir.dotProduct(normalVec) , 0.0f);								
        blinn =  Ogre::Math::Pow(blinn, scene->objects[currenObjId]->getBlinnN())*scene->objects[currenObjId]->getN()*coef;
		pixel += blinn * current.getSpecular() * scene->objects[currenObjId]->getSpecular();
	}
}

} //namespace rendering