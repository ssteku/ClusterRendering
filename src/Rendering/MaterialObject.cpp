#include "MaterialObject.hpp"

namespace rendering
{

    MaterialObject::MaterialObject(float const a[], float const d[], float const s[], float n, float phongN, float blinnN, ObjectTypes type, const unsigned id)
            : Object(a, d, s, id), n(n), phongN(phongN), blinnN(blinnN), type(type)
{	
}

} //namespace rendering