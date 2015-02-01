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
    Object(float const a[], float const d[], float const s[], const unsigned id);

    Ogre::ColourValue getAmbient() const;

    Ogre::ColourValue getDiffuse() const;

    Ogre::ColourValue getSpecular() const;

    unsigned getId() const;
	virtual std::pair< bool, float > intersects(const Ogre::Ray &ray)=0;

private:
	Ogre::ColourValue ambient;
	Ogre::ColourValue diffuse;
	Ogre::ColourValue specular;
    unsigned id_;
};

    using ObjectId = unsigned;
} //namespace rendering
#endif // _OBJECT_HPP_