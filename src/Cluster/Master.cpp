#include "Master.hpp"
#include <sys/stat.h>

using namespace std;

Master::Master(mpi::communicator& comm) : world(comm){
	createDirectoriesTree();
	frameManager.reset(new FramesManager(world,reader));

	frameManager->startBehavior();
}


void Master::createDirectoriesTree(){
	mkdir("images",  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir("movies", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir("logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void Master::manageWork(){
	frameManager->join();
}





