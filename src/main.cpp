#include <boost/mpi.hpp>
#include "Master.hpp"
#include "Slave.hpp"



namespace mpi = boost::mpi;
int main(int argc, char* argv[])
{
	mpi::environment env(argc, argv);
	mpi::communicator world;
	if (world.rank() == 0) {
		Master master(world);		
		master.startBehavior();
		master.join();
	} else {
		Slave slave(world);		
		slave.startBehavior();	
		slave.join();
	}


	return 0;
}

