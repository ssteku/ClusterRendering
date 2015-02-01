#ifndef _PLANE_HPP_
#define _PLANE_HPP_

#include "MaterialObject.hpp"
#include <OGRE/OgrePlane.h>
namespace rendering
{

class Plane : public MaterialObject
{
public:
    Plane(float const a[], float const d[], float const s[], float normalVec[], float dist, float n, float phongN, float blinnN, const unsigned id);
	const Ogre::Plane& getPlane();
	std::pair< bool, float > intersects(const Ogre::Ray &ray);
	Ogre::Vector3 getNormalVector(const Ogre::Vector3& intersecPoint);
private:
	Ogre::Plane plane;
};

} //namespace rendering

#endif // _PLANE_HPP_