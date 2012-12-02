#include "MaterialObject.hpp"

namespace rendering
{

MaterialObject::MaterialObject(const float a[],const float d[], const float s[], float n,float phongN, float blinnN, ObjectTypes type )
	: Object(a,d,s), n(n), phongN(phongN), blinnN(blinnN),type(type)
{	
}

} //namespace rendering