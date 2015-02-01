#ifndef _TRIANGLE_HPP_
#define _TRIANGLE_HPP_

#include "MaterialObject.hpp"
#include <OGRE/OgreVector3.h>
namespace rendering
{

class Triangle : public MaterialObject
{
public:
    Triangle(float const a[], float const d[], float const s[], float point1[], float point2[], float point3[], float nVec[], float n, float phongN, float blinnN, const unsigned id);
	const Ogre::Vector3& getP1();
	const Ogre::Vector3& getP2();
	const Ogre::Vector3& getP3();
	const Ogre::Vector3& getNormalVec();

	std::pair< bool, float > intersects(const Ogre::Ray &ray);
	Ogre::Vector3 getNormalVector(const Ogre::Vector3& intersecPoint);
	
private:
	Ogre::Vector3 p1,p2,p3,normalVec;
};

} //namespace rendering

#endif // _TRIANGLE_HPP_