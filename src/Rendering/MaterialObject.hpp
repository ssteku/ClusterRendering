#ifndef _MATERIALOBJECT__HPP_
#define _MATERIALOBJECT__HPP_

#include "Object.hpp"

namespace Ogre
{
	class Vector3;
}

namespace rendering
{

class MaterialObject : public Object
{
public:
	enum ObjectTypes {tBox,tPlane,tSphere,tTriangle};

	MaterialObject(const float a[],const float d[], const float s[], float n,float phongN, float blinnN, ObjectTypes type );
	virtual inline float getN()
	{
		return n;
	}

	virtual inline float getPhongN()
	{
		return phongN;
	}

	virtual inline float getBlinnN()
	{
		return blinnN;
	}

	ObjectTypes getType()
	{
		return type;
	}


	virtual Ogre::Vector3 getNormalVector(const Ogre::Vector3& intersecPoint) = 0;

protected:

	float n;
	float phongN;
	float blinnN;
	ObjectTypes type;
};

} //namespace rendering

#endif //_MATERIALOBJECT__HPP_