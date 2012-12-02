#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include <OGRE/OgreColourValue.h>
#include <utility>



class Ray;

namespace rendering
{

class Object
{
public:
	Object(const float a[],const float d[], const float s[]);

	Ogre::ColourValue& getAmbient();
	Ogre::ColourValue& getDiffuse();
	Ogre::ColourValue& getSpecular();

	virtual std::pair< bool, float > intersects(const Ogre::Ray &ray)=0;

private:
	Ogre::ColourValue ambient;
	Ogre::ColourValue diffuse;
	Ogre::ColourValue specular;
};

} //namespace rendering
#endif // _OBJECT_HPP_