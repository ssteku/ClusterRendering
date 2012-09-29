#include "WorkerObject.hpp"
#include <boost/function.hpp>
#include <boost/bind.hpp>

void WorkerObject::startBehavior(){
	executedThread = boost::thread(&WorkerObject::manageWork, this);  		
}

void WorkerObject::join(){
	executedThread.join();
}
