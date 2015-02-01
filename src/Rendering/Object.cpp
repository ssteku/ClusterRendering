#include "Object.hpp"

using namespace Ogre;

namespace rendering
{

    Object::Object(float const a[], float const d[], float const s[], const unsigned id)
            : ambient(a[0], a[1], a[2], 1.0), diffuse(d[0], d[1], d[2]), specular(s[0], s[1], s[2]), id_(id)
{

}


    ColourValue Object::getAmbient() const
{
	return ambient;
}

    ColourValue Object::getDiffuse() const
{
	return diffuse;
}

    ColourValue Object::getSpecular() const
{
	return specular;
}

    unsigned Object::getId() const {
        return id_;
    }
} //namespace rendering
