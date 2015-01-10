#include "Master.hpp"
#include <sys/stat.h>

using namespace std;

Master::Master(){
	createDirectoriesTree();
	frameManager.reset(new FramesManager());
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





