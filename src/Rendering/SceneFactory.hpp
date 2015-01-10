#ifndef _SCENEFACTORY_HPP_
#define _SCENEFACTORY_HPP_

#include <memory>

class Context;

namespace rendering
{

class Scene;

class SceneFactory
{
public:
	std::shared_ptr<Scene> createScene(Context* context);
};

} //namespace rendering
#endif // _SCENEFACTORY_HPP_