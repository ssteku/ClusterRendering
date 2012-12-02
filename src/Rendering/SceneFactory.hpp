#ifndef _SCENEFACTORY_HPP_
#define _SCENEFACTORY_HPP_
#include <boost/shared_ptr.hpp>

class Context;

namespace rendering
{

class Scene;

class SceneFactory
{
public:
	boost::shared_ptr<Scene> createScene(Context* context);
};

} //namespace rendering
#endif // _SCENEFACTORY_HPP_