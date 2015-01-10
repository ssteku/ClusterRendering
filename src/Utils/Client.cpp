#include "Client.hpp"
#include "Part.hpp"
#include "Context.h"
#include <iostream>

using namespace std;

enum message_tags {msg_data_packet, msg_broadcast_data, msg_finished};

Client::Client(int clientNumber,std::mutex& mpiMutex) :
	clientNr(clientNumber), hasTask(false)
{
	results.reset(new std::vector<char>());

}

void Client::manageWork()
{
//	while(1)
//	{
//		{
//			// cout<<"Wait for task"<<endl;
//			boost::mutex::scoped_lock lock(io_mutex);
//			while(!hasTask)
//			{
//				// cout<<"Waiting for t"<<endl;
//				taskAdded.wait(lock);
//			}
//		}
//
//		{
//			boost::mutex::scoped_lock mpiLock(mpiServiceMutex);
//
//	        boost::optional<mpi::status> l_status = world.iprobe(clientNr, clientNr) ;
//			if(l_status.is_initialized())
//			{
//	            world.recv(  l_status->source(), clientNr, *(results.get()));
//				hasTask = false;
//				cout<<"Client: Nr: "<<clientNr<<" Received data from Slave: "<<l_status->source()<<endl;
//				mpiLock.unlock();
//
//	   	 	}
//	   	 	else
//	   	 	{
//	   	 		mpiLock.unlock();
//		       	boost::this_thread::yield();
//	        	continue;
//	   	 	}
//	   	 }
//		boost::mutex::scoped_lock lock(io_mutex);
//		long frameRenderTime = task->savePart(results,task->getPartNumber());
//		callback(frameRenderTime);
//		task.reset();
//	}
}

void Client::render(std::shared_ptr<Part> task, boost::function<void (long)> callback)
{
	std::unique_lock<std::mutex> lock(io_mutex);
	this->task = task;
	this->callback = callback;
	hasTask = true;
	taskAdded.notify_all();
}

int Client::getNumber() const
{
	return clientNr;
}