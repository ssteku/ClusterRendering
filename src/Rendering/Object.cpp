#include "Object.hpp"

using namespace Ogre;

namespace rendering
{

Object::Object(const float a[],const float d[], const float s[])
	:ambient(a[0],a[1],a[2],1.0), diffuse(d[0],d[1],d[2]), specular(s[0],s[1],s[2])
{

}


ColourValue& Object::getAmbient()
{
	return ambient;
}
ColourValue& Object::getDiffuse()
{
	return diffuse;
}
ColourValue& Object::getSpecular()
{
	return specular;
}

} //namespace rendering
