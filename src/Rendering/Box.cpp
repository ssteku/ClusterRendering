#include "Box.hpp"
#include <OGRE/OgreMath.h> 

namespace rendering
{

	Box::Box(const float a[],const float d[], const float s[],float miVec[],float maxVec[], float n,float phongN, float blinnN)
		:MaterialObject(a,d,s,n,phongN, blinnN,MaterialObject::tBox), box(miVec[0],miVec[1],miVec[2],maxVec[0],maxVec[1],maxVec[2])
	{
	}

	const Ogre::AxisAlignedBox& Box::getBox()
	{
		return box;
	}

	std::pair< bool, float > Box::intersects(const Ogre::Ray &ray)
	{
		std::pair<bool,float> ret = Ogre::Math::intersects(ray,box);
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

	Ogre::Vector3 Box::getNormalVector(const Ogre::Vector3& intersecPoint)
	{
		//TODO
		//do poprawienia na vec normal sciany!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		return intersecPoint - box.getCenter(); 
	}

} //namespace rendering