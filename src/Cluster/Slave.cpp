#include "Slave.hpp"
#include "Context.h"
#include "Raytracer.h"
#include <boost/serialization/vector.hpp>

Slave::Slave(mpi::communicator& comm) : world(comm){
}

void Slave::manageWork(){
	Context c;
	while(1){
		mpi::request reqs[2];
		reqs[0] = world.irecv(0, 0, c);
		mpi::wait_all(reqs, reqs + 1);
		std::vector <float> pixels;
		Raytracer ray(&c);
		ray.Go(&pixels);
		reqs[1] = world.isend(0,world.rank(),pixels);	
		mpi::wait_all(reqs+1, reqs + 2);
	}
}


