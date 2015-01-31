#include "Slave.hpp"
#include "Context.h"
#include "Scene.hpp"
#include "SceneFactory.hpp"
#include "RayRenderer.hpp"


using namespace std;

Slave::Slave(){
}

void Slave::render(const Context &context) {
    Pixels pixels;
	std::shared_ptr<rendering::Scene> scene;

	rendering::RayRenderer rendRay(scene);
	rendRay.render(&pixels);
	scene.reset();
}


