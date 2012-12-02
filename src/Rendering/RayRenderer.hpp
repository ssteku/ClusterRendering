#ifndef _RAYRENDERER_HPP_
#define _RAYRENDERER_HPP_

#include <boost/shared_ptr.hpp>
#include "Scene.hpp"
#include <vector>
#include <OGRE/OgreColourValue.h>

namespace Ogre
{
	class Ray;
	class Vector3;
}

namespace rendering
{

class RayRenderer
{
public:
	RayRenderer(boost::shared_ptr<Scene> scene);
	void render(std::vector < float > *pixels);

private:
	void countPhong(Ogre::ColourValue& pixel, const Ogre::Ray& lightRay, 
		const Ogre::Ray& ray, const Ogre::Vector3& normalVec, Light& current, const int currenObjId, float coef);
	void countBlinn(Ogre::ColourValue& pixel, const Ogre::Ray& lightRay, 
		const Ogre::Ray& ray, const Ogre::Vector3& normalVec, Light& current, const int currenObjId, float coef);

	boost::shared_ptr<Scene> scene;
	const float MAX_DISTANCE;
	const int NO_OBJECT;
	const Ogre::ColourValue NO_COLOR;
	const int START_LEVEL;
	const float START_COEF;
	const float ANTYALIASING_FACTOR;

};

}//namespace rendering
#endif // _RAYRENDERER_HPP_