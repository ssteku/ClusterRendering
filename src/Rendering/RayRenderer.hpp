#ifndef _RAYRENDERER_HPP_
#define _RAYRENDERER_HPP_

#include <memory>
#include "Scene.hpp"
#include <vector>
#include <OGRE/OgreColourValue.h>
#include "DataDefs.hpp"
namespace Ogre
{
	class Ray;
	class Vector3;
}

namespace rendering
{

	class RayRenderer {
	public:
		RayRenderer(std::shared_ptr<Scene> scene);

        void render(Pixels *pixels);

	private:
        ObjectId findIntersectingObject(
                const MaterialObjectPtrVec &sceneObjects, const Ogre::Ray &ray) const;

        Ogre::ColourValue calculatePhongOffset(
                const float phongValue, const MaterialObjectPtr &currentObject, const Light &currentLight);

        Ogre::Vector3 calculatePhongDirection(const Ogre::Ray &lightRay, const Ogre::Vector3 &normalVec) const;

        float calculatePhongValue(
                const Ogre::Vector3 &phongDirection, const Ogre::Ray &ray,
                const MaterialObjectPtr &currentObject, const float coeficiency) const;

        Ogre::ColourValue countBlinnOffset(const float blinnValue,
                const Light &currentLight, const MaterialObjectPtr &currentObject);

        Ogre::Vector3 calculateBlinnDirection(const Ogre::Ray &lightRay, const Ogre::Ray &ray) const;

        float calculateBlinnValue(const Ogre::Vector3 &blinnDir, const Ogre::Vector3 &normalVec,
                const MaterialObjectPtr &currentObject, const float coeficiency) const;

		std::shared_ptr<Scene> scene;
		const float MAX_DISTANCE;
		const int NO_OBJECT;
		const Ogre::ColourValue NO_COLOR;
		const int START_LEVEL;
		const float START_COEF;
		const float ANTYALIASING_FACTOR;

	};

}//namespace rendering
#endif // _RAYRENDERER_HPP_