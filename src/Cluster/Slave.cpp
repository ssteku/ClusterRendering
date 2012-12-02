#include "Slave.hpp"
#include "Context.h"
#include "Raytracer.h"
#include <boost/serialization/vector.hpp>
#include "Scene.hpp" 
#include "SceneFactory.hpp"
#include <boost/shared_ptr.hpp>
#include <OGRE/OgreSphere.h>
#include "RayRenderer.hpp"

#include <iostream>

using namespace std;

Slave::Slave(mpi::communicator& comm) : world(comm){
}

void Slave::manageWork(){
	Context c;
	while(1){
		mpi::request reqs[2];
		reqs[0] = world.irecv(0, 0, c);
		mpi::wait_all(reqs, reqs + 1);
		std::vector <float> pixels;

		rendering::SceneFactory factory;
		boost::shared_ptr<rendering::Scene> scene;

		scene = factory.createScene(&c);

		rendering::RayRenderer rendRay(scene);
		rendRay.render(&pixels);		
		// Raytracer ray(&c);
		// ray.Go(&pixels);
		reqs[1] = world.isend(0,world.rank(),pixels);	
		mpi::wait_all(reqs+1, reqs + 2);
		scene.reset();
		
	}
}


