#ifndef _SPHERE_HPP_
#define _SPHERE_HPP_

#include "MaterialObject.hpp"
#include <OGRE/OgreSphere.h>

namespace rendering
{

class Sphere : public MaterialObject
{
public:
	Sphere(const float a[],const float d[], const float s[],float r,float pos[], float n, float phongN, float blinnN);
	const Ogre::Sphere& getSphere();
	std::pair< bool, float > intersects(const Ogre::Ray &ray);
	Ogre::Vector3 getNormalVector(const Ogre::Vector3& intersecPoint);
private:
	Ogre::Sphere sphere;	
	
};

} //namespace rendering
#endif // _SPHERE_HPP_