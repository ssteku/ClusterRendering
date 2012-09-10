#include "WorkerObject.hpp"

void WorkerObject::startBehavior(){
	executedThread = boost::thread(&WorkerObject::manageWork, this);  		
}

void WorkerObject::join(){
	executedThread.join();
}
