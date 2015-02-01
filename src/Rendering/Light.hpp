#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

#include "Object.hpp"
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreSphere.h>
namespace rendering
{

class Light : public Object
{
public:
    Light(float const a[], float const d[], float const s[], float r, float pos[], const unsigned id);
	const Ogre::Sphere& getSphere();
	std::pair< bool, float > intersects(const Ogre::Ray &ray);
private:
	Ogre::Sphere sphere;
};

} //namespace rendering
#endif // _LIGHT_HPP_