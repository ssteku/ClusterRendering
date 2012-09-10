#include "Slave.hpp"
#include "../Utils/Context.h"
#include "../Rendering/Raytracer.h"

#include <cstdio>
#include <cmath>
#include <boost/serialization/vector.hpp>

Slave::Slave(mpi::communicator& comm) : world(comm){
}

void Slave::manageWork(){
	Context c;
	while(1){
		mpi::request reqs[2];

		reqs[0] = world.irecv(0, 0, c);
		mpi::wait_all(reqs, reqs + 1);

		 // std::cout<<"Received something! z maxRBounce : "<<c.maxRayBounce<<std::endl;
		// std::cout<<"Sphere 0 : position[0] : "<<c.spheres[0].position[0]<<std::endl;
		std::vector < float > pixels;
		Raytracer ray(&c);

		ray.Go(&pixels);
		// std::cout <<"Wielkosc vec wynikow Slave: "<<pixels.size()<<std::endl;
		reqs[1] = world.isend(0,world.rank(),pixels);	
		mpi::wait_all(reqs+1, reqs + 2);

	}
}


