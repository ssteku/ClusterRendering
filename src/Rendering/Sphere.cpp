#include "Sphere.hpp"

#include <OGRE/OgreRay.h>
namespace rendering
{

    Sphere::Sphere(float const a[], float const d[], float const s[], float r, float pos[], float n, float phongN, float blinnN, const unsigned id)
            : MaterialObject(a, d, s, n, phongN, blinnN, MaterialObject::tSphere, id)
{
	Ogre::Vector3 vec(pos[0],pos[1],pos[2]);
	sphere = Ogre::Sphere(vec,r); 
}
const Ogre::Sphere& Sphere::getSphere()
{
	return sphere;
}
std::pair< bool, float > Sphere::intersects(const Ogre::Ray &ray)
{
//    std::pair<bool, float> ret = Ogre::Math::intersects(ray, sphere);
    auto ret = checkIntersection(ray, sphere);
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

    std::pair<bool, float> Sphere::checkIntersection(const Ogre::Ray &ray, const Ogre::Sphere &sphere) {
        const auto &raydir = ray.getDirection();
        // Adjust ray origin relative to sphere center
        const auto &rayorig = ray.getOrigin() - sphere.getCenter();
        const auto radius = sphere.getRadius();

        // Check origin inside first
        const auto radiusPow = radius * radius;
        if (rayorig.squaredLength() <= radiusPow) {
            return std::make_pair(true, 0);
        }

        // Mmm, quadratics
        // Build coeffs which can be used with std quadratic solver
        // ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
        const auto a = raydir.dotProduct(raydir);
        const auto b = 2 * rayorig.dotProduct(raydir);
        const auto c = rayorig.dotProduct(rayorig) - radiusPow;

        // Calc determinant
        const auto d = (b * b) - (4 * a * c);
        if (d < 0) {
            // No intersection
            return std::make_pair(false, 0);
        }
        else {
            // BTW, if d=0 there is one intersection, if d > 0 there are 2
            // But we only want the closest one, so that's ok, just use the
            // '-' version of the solver
            float t = (-b - std::sqrt(d)) / (2 * a);
            if (t < 0)
                t = (-b + std::sqrt(d)) / (2 * a);
            return std::make_pair(true, t);
        }


    }

Ogre::Vector3 Sphere::getNormalVector(const Ogre::Vector3& intersecPoint)
{
	return intersecPoint - sphere.getCenter();
}

}// namespace rendering