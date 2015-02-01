#include "Plane.hpp"

namespace rendering
{

    Plane::Plane(float const a[], float const d[], float const s[], float normalVec[], float dist, float n, float phongN, float blinnN, const unsigned id)
            : MaterialObject(a, d, s, n, phongN, blinnN, MaterialObject::tPlane, id),
              plane(normalVec[0], normalVec[1], normalVec[2], dist)
{
}
const Ogre::Plane& Plane::getPlane()
{
	return plane;
}
std::pair< bool, float > Plane::intersects(const Ogre::Ray &ray)
{
	std::pair<bool,float> ret = Ogre::Math::intersects(ray,plane);
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

Ogre::Vector3 Plane::getNormalVector(const Ogre::Vector3& intersecPoint)
{
	return plane.normal;
}

} //namespace rendering