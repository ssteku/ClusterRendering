#include <SceneFactory.hpp>
#include <RayRenderer.hpp>
#include "Part.hpp"

using namespace std;

Part::Part(Context cont, unsigned int partNr) :
		context(cont), partNumber(partNr)
{

}

Pixels Part::calculatePart()
{
	std::vector<char> pixels;
	rendering::SceneFactory factory;
	std::shared_ptr<rendering::Scene> scene;

	scene = factory.createScene(&context);

	rendering::RayRenderer rendRay(scene);
	rendRay.render(&pixels);
	return pixels;
}



