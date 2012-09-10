/*
 * Raytracer.h
 *
 *  Created on: 29-03-2012
 *      Author: Kuba
 */

#include "../Utils/Context.h"

#ifndef RAYTRACER_H_
#define RAYTRACER_H_

class Raytracer {
	typedef float point[3];
	typedef float color[3];
	typedef float myVector[3];
	struct myColor { float R; float G; float B; };

	enum INTERSECT_STATUS {
	  LIGHT_SOURCE, NO_INTERSECTION, INTERSECTION
	};
	const static float MINIMAL_LIGHT_VISIBILITY = 0.01;
	const static float LIGHT_DISTANCE_PARAM_A = 0.001;
	const static float LIGHT_DISTANCE_PARAM_B = 0.01;
	const static float LIGHT_DISTANCE_PARAM_C = 1.0;

	float MAX_LIGHT_DISTANCE;
	Context *context;
public:
	Raytracer(Context *cont);
	virtual ~Raytracer();
	void Go(std::vector < float > *ret);
private:
	void Trace(point p, myVector d, int step, color ret);
	void Phong(point q, myVector n, myVector V, unsigned int objectId, color ret);
	void Normal(point p, point c, myVector ret);
	void Reflect(point p, point q, myVector n, myVector ret);
	void Intersect(point p, myVector d, INTERSECT_STATUS *status, int *objectId, point ret);
	void IntersectWithSphere(point p, myVector d, INTERSECT_STATUS *status, int objectId, point ret);
	void IntersectWithLight(point p, myVector d, INTERSECT_STATUS *status, int objectId);

	float dotProduct(point p1, point p2);
	float Distance(point p1, point p2);

	void Normalization(point p);

	float Fatt(float d);
	void calculateMLD(); //max light distance
};

#endif /* RAYTRACER_H_ */
