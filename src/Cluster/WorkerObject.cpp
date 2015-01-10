#include "WorkerObject.hpp"


void WorkerObject::startBehavior(){
	executedThread = std::thread(&WorkerObject::manageWork, this);
}

void WorkerObject::join(){
	executedThread.join();
}
