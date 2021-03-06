#include "Sphere.hpp"

#include <OGRE/OgreVector3.h>
#include <OGRE/OgreMath.h> 
#include <OGRE/OgreRay.h> 
namespace rendering
{

Sphere::Sphere(const float a[],const float d[], const float s[],float r,float pos[], float n, float phongN, float blinnN)
	:MaterialObject(a,d,s,n,phongN,blinnN,MaterialObject::tSphere)
{
	Ogre::Vector3 vec(pos[0],pos[1],pos[2]);
	sphere = Ogre::Sphere(vec,r); 
}
const Ogre::Sphere& Sphere::getSphere()
{
	return sphere;
}
std::pair< bool, float > Sphere::intersects(const Ogre::Ray &ray)
{
	std::pair<bool,float> ret = Ogre::Math::intersects(ray,sphere);
	if(ret.first && ret.second > 0.01)
	{
		return ret;
	}
	else
	{
		ret.first = false;
		ret.second = -1;
		return ret;
	}
}

Ogre::Vector3 Sphere::getNormalVector(const Ogre::Vector3& intersecPoint)
{
	return intersecPoint - sphere.getCenter();
}

}// namespace rendering