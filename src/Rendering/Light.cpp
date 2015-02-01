#include "Light.hpp"

namespace rendering
{

    Light::Light(float const a[], float const d[], float const s[], float r, float pos[], const unsigned id)
            : Object(a, d, s, id), sphere(Ogre::Vector3(pos[0], pos[1], pos[2]), r)
{

}

const Ogre::Sphere& Light::getSphere()
{
	return sphere;
}

std::pair< bool, float > Light::intersects(const Ogre::Ray &ray)
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

} //namespace rendering