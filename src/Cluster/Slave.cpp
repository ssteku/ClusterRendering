#include "Slave.hpp"
#include "Context.h"
#include "Scene.hpp"
#include "SceneFactory.hpp"
#include "RayRenderer.hpp"


using namespace std;

Slave::Slave(){
}

void Slave::render(const Context &context) {
	std::vector<char> pixels;
	rendering::SceneFactory factory;
	std::shared_ptr<rendering::Scene> scene;

//	scene = factory.createScene(&context);

	rendering::RayRenderer rendRay(scene);
	rendRay.render(&pixels);
	// Raytracer ray(&c);
	// ray.Go(&pixels);
	scene.reset();
}


