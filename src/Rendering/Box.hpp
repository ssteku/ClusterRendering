#ifndef _BOX_HPP_
#define _BOX_HPP_

#include "MaterialObject.hpp"
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreAxisAlignedBox.h>
namespace rendering
{

class Box : public MaterialObject
{
public:
	Box(const float a[],const float d[], const float s[],float miVec[],float maxVec[], float n,float phongN, float blinnN);
	const Ogre::AxisAlignedBox& getBox();
	std::pair< bool, float > intersects(const Ogre::Ray &ray);
	Ogre::Vector3 getNormalVector(const Ogre::Vector3& intersecPoint);

private:
	Ogre::AxisAlignedBox box;
};

} //namespace rendering
#endif // _BOX_HPP_