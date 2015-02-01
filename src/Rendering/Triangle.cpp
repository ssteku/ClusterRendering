#include "Triangle.hpp"
#include <OGRE/OgreRay.h>

using namespace std;
namespace rendering
{

    Triangle::Triangle(float const a[], float const d[], float const s[], float point1[], float point2[], float point3[], float nVec[], float n, float phongN, float blinnN, const unsigned id)
            : MaterialObject(a, d, s, n, phongN, blinnN, MaterialObject::tTriangle, id),
              p1(point1[0], point1[1], point1[2]),
              p2(point2[0], point2[1], point2[2]),
              p3(point3[0], point3[1], point3[2]),
	normalVec(nVec[0],nVec[1],nVec[2])
{
}
const Ogre::Vector3& Triangle::getP1()
{
	return p1;
}
const Ogre::Vector3& Triangle::getP2()
{
	return p2;
}
const Ogre::Vector3& Triangle::getP3()
{
	return p3;
}
const Ogre::Vector3& Triangle::getNormalVec()
{
	Ogre::Vector3 n = Ogre::Math::calculateBasicFaceNormal( p1,p2,p3);
	// n.normalise();

	// Ogre::Vector3 u,v,n;
	// u = p2 - p1;
	// v = p3 - p1;
	// normalVec = u.crossProduct(v);
	// normalVec.normalise();
	return n;
}

std::pair< bool, float > Triangle::intersects(const Ogre::Ray &ray)
{


	std::pair<bool,float> ret = Ogre::Math::intersects(ray, p1,p2,p3,true,true);
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




Ogre::Vector3 Triangle::getNormalVector(const Ogre::Vector3& intersecPoint)
{
	Ogre::Vector3 n = Ogre::Math::calculateBasicFaceNormal( p1,p2,p3);
	return n;
}

} //namespace rendering