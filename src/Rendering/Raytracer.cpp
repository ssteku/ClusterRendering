/*
 * Raytracer.cpp
 *
 *  Created on: 29-03-2012
 *      Author: Kuba
 */

#include "Raytracer.h"
#include <cmath>
#include <iostream>
Raytracer::Raytracer(Context *cont) : context(cont) {
	calculateMLD();

}

Raytracer::~Raytracer() {}

float Raytracer::dotProduct(point p1, point p2) {
	return (p1[0]*p2[0]+p1[1]*p2[1]+p1[2]*p2[2]);
}

void Raytracer::IntersectWithLight(point p, myVector d,
		INTERSECT_STATUS *status, int objectId) {
	float a = dotProduct(d, d);
	float b = 2*(d[0]*(p[0]-context->lights[objectId].position[0]) + d[1]*(p[1]-context->lights[objectId].position[1]) + d[2]*(p[2]-context->lights[objectId].position[2]));
	float tmp = (p[0]-context->lights[objectId].position[0]);
	tmp *= tmp;
	float c = tmp;
	tmp = (p[1]-context->lights[objectId].position[1]);
	tmp *= tmp;
	c += tmp;
	tmp = (p[2]-context->lights[objectId].position[2]);
	tmp *= tmp;
	c += tmp;
	c -= context->lights[objectId].r*context->lights[objectId].r;
	float delta = b*b - 4*a*c;
	if( delta >= 0 ) {
	  *status = LIGHT_SOURCE;
	} else {
	  *status = NO_INTERSECTION;
	}
}

void Raytracer::Phong(point q, myVector n, myVector V, unsigned int objectId, color ret) {
	  std::vector < myColor > singlePhongColors;
	  color ambientSum = {0.0, 0.0, 0.0};
	  Normalization(V);
	  for(unsigned int Li = 0; Li < context->lights.size(); ++Li) {
		float lightLen = Distance(q, context->lights[Li].position);
		if(lightLen >= MAX_LIGHT_DISTANCE) continue;
		bool isVisible = true;
		myVector L = {context->lights[Li].position[0]-q[0],
					context->lights[Li].position[1]-q[1],
					context->lights[Li].position[2]-q[2]};
		Normalization(L);
		for(unsigned int Si = 0; Si < context->spheres.size(); ++Si) {
		  if(Si == objectId) continue;
		  INTERSECT_STATUS status;
		  point tmp;
		  IntersectWithSphere(context->lights[Li].position, L, &status, Si, tmp);
		  if(status == INTERSECTION) {
			myVector tmp2 =  {context->lights[Li].position[0]-tmp[0],
						   context->lights[Li].position[1]-tmp[1],
						   context->lights[Li].position[2]-tmp[2]};
			Normalization(tmp2);
			if(tmp2[0] != L[0] && tmp2[1] != L[1] && tmp2[2] != L[2]) continue;
			if(Distance(tmp, context->lights[Li].position) < lightLen) {
			  isVisible = false;
			  break;
			}
		  }
		}
		if(!isVisible) continue;
		color tmpC = {0.0, 0.0, 0.0};
		myVector R;
		Reflect(context->lights[Li].position, q, n,  R);
		Normalization(R);
		float RdotV = dotProduct(R, V);
		if(RdotV >  0) {

		  RdotV = pow(RdotV, float(context->spheres[objectId].n));
		  tmpC[0] = context->spheres[objectId].specular[0]*context->lights[Li].specular[0]*RdotV;
		  tmpC[1] = context->spheres[objectId].specular[1]*context->lights[Li].specular[1]*RdotV;
		  tmpC[2] = context->spheres[objectId].specular[2]*context->lights[Li].specular[2]*RdotV;
		}
		float NdotL = dotProduct(n, L);
		if(NdotL > 0) {
		  tmpC[0] += context->spheres[objectId].diffuse[0]*context->lights[Li].diffuse[0]*NdotL;
		  tmpC[1] += context->spheres[objectId].diffuse[1]*context->lights[Li].diffuse[1]*NdotL;
		  tmpC[2] += context->spheres[objectId].diffuse[2]*context->lights[Li].diffuse[2]*NdotL;
		}
		float dist = Fatt(lightLen);
		tmpC[0] *= dist;
		tmpC[1] *= dist;
		tmpC[2] *= dist;

		ambientSum[0] = context->lights[Li].ambient[0];
		ambientSum[1] = context->lights[Li].ambient[1];
		ambientSum[2] = context->lights[Li].ambient[2];
		myColor tmpColor;
		tmpColor.R = tmpC[0];
		tmpColor.G = tmpC[1];
		tmpColor.B = tmpC[2];
		singlePhongColors.push_back(tmpColor);
	  }
	  ret[0] = context->spheres[objectId].ambient[0]*context->globalLight[0];
	  ret[1] = context->spheres[objectId].ambient[1]*context->globalLight[1];
	  ret[2] = context->spheres[objectId].ambient[2]*context->globalLight[2];
	  int lightCnt = singlePhongColors.size();
	  for(int i = 0; i < lightCnt; ++i) {
		ret[0] += singlePhongColors[i].R;
		ret[1] += singlePhongColors[i].G;
		  ret[2] += singlePhongColors[i].B;
	  }
	  if(lightCnt > 0) {
		ret[0] += (ambientSum[0]/lightCnt)*context->spheres[objectId].ambient[0];
		ret[1] += (ambientSum[1]/lightCnt)*context->spheres[objectId].ambient[1];
		ret[2] += (ambientSum[2]/lightCnt)*context->spheres[objectId].ambient[2];
	  }
}

void Raytracer::IntersectWithSphere(point p, myVector d,
		INTERSECT_STATUS *status, int objectId, point ret) {
	float a = dotProduct(d, d);
	float b = 2*(d[0]*(p[0]-context->spheres[objectId].position[0]) + d[1]*(p[1]-context->spheres[objectId].position[1]) + d[2]*(p[2]-context->spheres[objectId].position[2]));
	float tmp = (p[0]-context->spheres[objectId].position[0]);
	tmp *= tmp;
	float c = tmp;
	tmp = (p[1]-context->spheres[objectId].position[1]);
	tmp *= tmp;
	c += tmp;
	tmp = (p[2]-context->spheres[objectId].position[2]);
	tmp *= tmp;
	c += tmp;
	c -= context->spheres[objectId].r*context->spheres[objectId].r;
	float delta = b*b - 4*a*c;
	ret[0] = 0; ret[1] = 0; ret[2] = 0;
	if( delta >= 0 ) {
	  float r = (-b-sqrt(delta))/(2*a);
	  *status = INTERSECTION;
	  ret[0] = p[0] + r*d[0];
	  ret[1] = p[1] + r*d[1];
	  ret[2] = p[2] + r*d[2];
	} else {
	  *status = NO_INTERSECTION;
	}
}

void Raytracer::Trace(point p, myVector d, int step, color ret) {
	color local, reflected;
	point q;
	myVector n,r;
	INTERSECT_STATUS status;
	int objectId;

	if(step > context->maxRayBounce) {
		ret[0] = context->background[0];
		ret[1] = context->background[1];
		ret[2] = context->background[2];
		return;
	}
	Intersect(p, d, &status, &objectId, q);
	if(status == LIGHT_SOURCE) {
//		float lattr = Fatt(Distance(p, context->lights[objectId].position));
//		ret[0] = context->lights[objectId].ambient[0] +context->lights[objectId].diffuse[0] + context->lights[objectId].specular[0]; //
//		ret[1] = context->lights[objectId].ambient[1] +context->lights[objectId].diffuse[1] + context->lights[objectId].specular[1];//
//		ret[2] = context->lights[objectId].ambient[2] +context->lights[objectId].diffuse[2] + context->lights[objectId].specular[2];//
//		ret[0] *= lattr;
//		ret[1] *= lattr;
//		ret[2] *= lattr;
		ret[0] = 0.0; ret[1] = 0.0; ret[2] = 0.0; // tak jest chyba lepiej
		return;
	}
	if(status == NO_INTERSECTION) {
		ret[0] = context->background[0];
		ret[1] = context->background[1];
		ret[2] = context->background[2];
		return;
	}
	Normal(q, context->spheres[objectId].position, n);
	Reflect(p, q, n, r);
	Phong(q, n, d, objectId, local);
	Trace(q, r, step+1, reflected);

	ret[0] = local[0] + reflected[0];
	ret[1] = local[1] + reflected[1];
	ret[2] = local[2] + reflected[2];
}

float Raytracer::Distance(point p1, point p2) {
	point tmp = {p1[0]-p2[0], p1[1]-p2[1], p1[2]-p2[2]};
	float ret;
	ret = tmp[0]*tmp[0] + tmp[1]*tmp[1] + tmp[2]*tmp[2];
	return sqrt(ret);
}

void Raytracer::Normalization(point p) {
	float d =0.0;
	int i;
	for(i=0; i<3; ++i)
		d+=p[i]*p[i];
	d=sqrt(d);
	if(d>0.0)
		for(i=0; i<3; ++i)
			p[i]/=d;
}

void Raytracer::Normal(point p, point c, myVector ret) {
	ret[0] = p[0] - c[0];
	ret[1] = p[1] - c[1];
	ret[2] = p[2] - c[2];
	Normalization(ret);
}

void Raytracer::Reflect(point p, point q, myVector n, myVector ret) {
	myVector l;
	float costheta;
	l[0] = p[0]-q[0];
	l[1] = p[1]-q[1];
	l[2] = p[2]-q[2];
	Normalization(l);
	costheta = dotProduct(l, n);
	ret[0] = 2*costheta*n[0] - l[0];
	ret[1] = 2*costheta*n[1] - l[1];
	ret[2] = 2*costheta*n[2] - l[2];
}

float Raytracer::Fatt(float d) {
	float c = LIGHT_DISTANCE_PARAM_C;
	float b = LIGHT_DISTANCE_PARAM_B;
	float a = LIGHT_DISTANCE_PARAM_A;
	float tmp = c + b*d + a*d*d;
	tmp = 1/tmp;
	if(tmp >= 1) return 1;
	if(tmp < MINIMAL_LIGHT_VISIBILITY) return 0.0;
	return tmp;
}

void Raytracer::Intersect(point p, myVector d, INTERSECT_STATUS *status,
		int *objectId, point ret) {
	INTERSECT_STATUS tmpstatus = NO_INTERSECTION;
	point tmpret = {0.0, 0.0, 0.0};
	float min_distance = 1000000.0;
	*status = NO_INTERSECTION;
	for(unsigned int Si = 0; Si < context->spheres.size(); ++Si) {
	  IntersectWithSphere(p, d, status, Si, ret);
	  if(*status == NO_INTERSECTION) continue;

	  myVector tmp = {ret[0]-p[0], ret[1]-p[1], ret[2]-p[2]};
	  if( ((tmp[0] >= 0 && d[0] >= 0) || (tmp[0] < 0 && d[0] < 0)) &&
	      ((tmp[1] >= 0 && d[1] >= 0) || (tmp[1] < 0 && d[1] < 0)) &&
	      ((tmp[2] >= 0 && d[2] >= 0) || (tmp[2] < 0 && d[2] < 0))  ) {
	     float tmpdist = Distance(p, ret);
	     if(tmpdist < min_distance) {
	      min_distance = tmpdist;
	      *objectId = Si;
	      tmpstatus = INTERSECTION;
	      tmpret[0] = ret[0];
	      tmpret[1] = ret[1];
	      tmpret[2] = ret[2];
	     }
	  }
	}
	*status = tmpstatus;
	ret[0] = tmpret[0];
	ret[1] = tmpret[1];
	ret[2] = tmpret[2];
	float min_Ldistance = 100000.0;
	for(unsigned int Li = 0; Li < context->lights.size(); ++Li) {
		 IntersectWithLight(p, d, &tmpstatus, Li);
		 if(tmpstatus == NO_INTERSECTION) continue;
		 myVector tmp = {context->lights[Li].position[0]-p[0], context->lights[Li].position[1]-p[1], context->lights[Li].position[2]-p[2]};
		 if( ((tmp[0] >= 0 && d[0] >= 0) || (tmp[0] < 0 && d[0] < 0)) &&
		  ((tmp[1] >= 0 && d[1] >= 0) || (tmp[1] < 0 && d[1] < 0)) &&
		  ((tmp[2] >= 0 && d[2] >= 0) || (tmp[2] < 0 && d[2] < 0))  ) {
		//jest swiatlo
		  float tmpdist = Distance(p, context->lights[Li].position);
		  if(tmpdist < min_Ldistance && tmpdist < min_distance) { //sprawdzenie czy ten punk swiatla jest blizszy niz poprzedni, oraz czy jest blizej niz obiekt
			*status = LIGHT_SOURCE;
			*objectId = Li;
			min_Ldistance = tmpdist;
		  }
		}
	}
}
void Raytracer::calculateMLD() {
	float c = LIGHT_DISTANCE_PARAM_C-(1/MINIMAL_LIGHT_VISIBILITY);
	float b = LIGHT_DISTANCE_PARAM_B;
	float a = LIGHT_DISTANCE_PARAM_A;
	float delta = b*b-4*a*c;
	MAX_LIGHT_DISTANCE = (-b+sqrt(delta))/(2*a);
}

void Raytracer::Go(std::vector< float > *ret) {
	int  x, y;           // pozycja rysowanego piksela "ca�kowitoliczbowa"
	float x_fl, y_fl;    // pozycja rysowanego piksela "zmiennoprzecinkowa"
	float x_flTmp, y_flTmp;
	float x_fbase, y_fbase; //
	int im_size_x_2;       // po�owa rozmiaru obrazu w pikselach
	int im_size_y_2;
	
	cout<<"total size: "<<3*(context->window[1][1]-context->window[0][1])*(context->window[1][0]-context->window[0][0])<<endl;

	cout<<"dim0: "<<context->dimension[0]<<" dim1: "<<context->dimension[1]<<endl;
    cout<<"win00: "<<context->window[0][0]<<endl;
    cout<<"win01: "<<context->window[0][1]<<endl;
    cout<<"win10: "<<context->window[1][0]<<endl;
    cout<<"win11: "<<context->window[1][1]<<endl;

	point starting_point;
	point starting_direction = {
			-context->cameraZ[0],
			-context->cameraZ[1],
			-context->cameraZ[2]}; // wektor opisuj�cy kierunek promienia
    im_size_x_2 = context->dimension[0]/2;    // obliczenie po�owy rozmiaru obrazu w pikselach
    im_size_y_2 = context->dimension[1]/2;
    x_fbase = (context->dimension[0]/context->viewportSize[0]);
    y_fbase = (context->dimension[1]/context->viewportSize[1]);
    ret->reserve(3*(context->window[1][1]-context->window[0][1])*(context->window[1][0]-context->window[0][0]));
    for(y = im_size_y_2 - (context->window[0][1]); y > im_size_y_2 - (context->window[1][1]); --y) {
    	for(x = -im_size_x_2 + (context->window[0][0]); x < -im_size_x_2 + (context->window[1][0]); ++x) {
            x_flTmp = (float)x/x_fbase;
            y_flTmp = (float)y/y_fbase;

            color pixel = {0.0, 0.0, 0.0};
            color tmpPixel = {0.0, 0.0, 0.0};
           // for(int tmpI = 0; tmpI <= 1; tmpI++) {
            //	for(int tmpJ = -1; tmpJ <= 0; tmpJ++) {
            		x_fl = x_flTmp; //+ (tmpI/x_fbase);
            		y_fl = y_flTmp;// + (tmpJ/y_fbase);
            		starting_point[0] = context->cameraPosition[0] + (x_fl*context->cameraX[0] + y_fl*context->cameraY[0]);
					starting_point[1] = context->cameraPosition[1] + (x_fl*context->cameraX[1] + y_fl*context->cameraY[1]);
					starting_point[2] = context->cameraPosition[2] + (x_fl*context->cameraX[2] + y_fl*context->cameraY[2]);// transformacja wsp kamery na globalne

					Trace(starting_point, starting_direction, 0, tmpPixel);
					pixel[0] += tmpPixel[0];
					pixel[1] += tmpPixel[1];
					pixel[2] += tmpPixel[2];
            //	}
            //}


            ret->push_back(pixel[0]);
            ret->push_back(pixel[1]);
            ret->push_back(pixel[2]);
    	}
    }
}

