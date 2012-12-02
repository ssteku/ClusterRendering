#include "SceneFactory.hpp"
#include "Scene.hpp"
#include "Context.h"
#include "Light.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Box.hpp"
#include "Triangle.hpp"

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreColourValue.h>
#include <iostream>

using namespace std;
namespace rendering
{
	
boost::shared_ptr<Scene> SceneFactory::createScene(Context* context)
{
	boost::shared_ptr<Scene> scene(new Scene());

	scene->dimension[0] = context->dimension[0];
	scene->dimension[1] = context->dimension[1];
	scene->window[0][0] = context->window[0][0];
	scene->window[0][1] = context->window[0][1];
	scene->window[1][0] = context->window[1][0];
	scene->window[1][1] = context->window[1][1];
	scene->antyAliasing = context->antyAliasing;
	cout<<"Antyalias scenecreator: "<<context->antyAliasing<<endl;
	scene->viewportSize = 
		Ogre::Vector3(context->viewportSize[0],context->viewportSize[1],context->viewportSize[2]);
	scene->background = 
		Ogre::ColourValue(context->background[0],context->background[1],context->background[2]);
	scene->globalLight = 
		Ogre::ColourValue(context->globalLight[0],context->globalLight[1],context->globalLight[2]);
	// cout<<"Glob light factory:"<<context->globalLight[0]<<context->globalLight[1]<<context->globalLight[2]<<endl;
	scene->cameraPosition =
		Ogre::Vector3(context->cameraPosition[0],context->cameraPosition[1],context->cameraPosition[2]);
	scene->cameraX =
		Ogre::Vector3(context->cameraX[0],context->cameraX[1],context->cameraX[2]);
	scene->cameraY =
		Ogre::Vector3(context->cameraY[0],context->cameraY[1],context->cameraY[2]);
	scene->cameraZ =
		Ogre::Vector3(context->cameraZ[0],context->cameraZ[1],context->cameraZ[2]);
	scene->maxRayBounce = context->maxRayBounce;
	scene->cameraId = context->cameraId;

	int actualObjectSize;

	scene->lights.resize(context->lights.size());
	for(unsigned int i=0;i<context->lights.size();++i)
	{
		// cout << "Light #" << context->lights[i].id << "\t";
  //       cout << context->lights[i].position[0] << "\t" << context->lights[i].position[1] << "\t" << context->lights[i].position[2] << "\t" << context->lights[i].ambient[0] << "\t" << context->lights[i].ambient[1] << "\t" << context->lights[i].ambient[2] << "\t" << context->lights[i].specular[0] << "\t" << context->lights[i].specular[1] << "\t" << context->lights[i].specular[2] << "\t" << context->lights[i].diffuse[0] << "\t" << context->lights[i].diffuse[1] << "\t" << context->lights[i].diffuse[2] << endl;
		scene->lights[i].reset(
			new Light(context->lights[i].ambient,context->lights[i].diffuse,context->lights[i].specular,context->lights[i].r,
			context->lights[i].position));
	}

	scene->objects.resize(context->spheres.size()
		+context->planes.size()+context->triangles.size()
		+context->boxes.size());
	
	actualObjectSize = 0;
	unsigned int i = 0;
	for(unsigned int g = 0; i<context->spheres.size()+actualObjectSize;++i,++g)
	{
		// cout<<"Sphere:"<<endl;
		// cout << context->spheres[g].position[0] << "\t" << context->spheres[g].position[1] << "\t" << context->spheres[g].position[2] << "\t" << context->spheres[g].ambient[0] << "\t" << context->spheres[g].ambient[1] << "\t" << context->spheres[g].ambient[2] << "\t" << context->spheres[g].specular[0] << "\t" << context->spheres[g].specular[1] << "\t" << context->spheres[g].specular[2] << "\t" << context->spheres[g].diffuse[0] << "\t" << context->spheres[g].diffuse[1] << "\t" << context->spheres[g].diffuse[2] << "\t" << context->spheres[g].n << "\t" << "\t"<< context->spheres[g].phongN<< context->spheres[g].blinnN << "\t"<< endl;

		scene->objects[i].reset(
			new Sphere(context->spheres[g].ambient,context->spheres[g].diffuse,context->spheres[g].specular,
				context->spheres[g].r,context->spheres[g].position, context->spheres[g].n,
				context->spheres[g].phongN, context->spheres[g].blinnN ));
	}

	actualObjectSize = i;

	for(unsigned int g = 0; i<context->planes.size()+actualObjectSize;++i,++g)
	{
		scene->objects[i].reset(
			new Plane(context->planes[g].ambient,context->planes[g].diffuse,context->planes[g].specular,
				context->planes[g].normalVector,context->planes[g].distance, context->planes[g].n,
				context->planes[g].phongN, context->planes[g].blinnN ));
	}

	actualObjectSize = i;

	for(unsigned int g = 0; i<context->triangles.size()+actualObjectSize;++i,++g)
	{

		scene->objects[i].reset(
			new Triangle(context->triangles[g].ambient,context->triangles[g].diffuse,context->triangles[g].specular,
				context->triangles[g].point1,context->triangles[g].point2, context->triangles[g].point3,
				context->triangles[g].normalVec, context->triangles[g].n,
				context->triangles[g].phongN, context->triangles[g].blinnN ));
		// cout<<"Triangle phong: "<<context->triangles[g].phongN<<" blinn: "<<context->triangles[g].blinnN<<" n: "<<context->triangles[g].n<<endl;
	}

	actualObjectSize = i;

	for(unsigned int g = 0; i<context->boxes.size()+actualObjectSize;++i,++g)
	{
		scene->objects[i].reset(
			new Box(context->boxes[g].ambient,context->boxes[g].diffuse,context->boxes[g].specular,
				context->boxes[g].minVector,context->boxes[g].maxVector, context->boxes[g].n,
				context->boxes[g].phongN, context->boxes[g].blinnN ));
	}
	return scene;
}

} //namespace rendering