#include "Client.hpp"
#include "Part.hpp"
#include "Context.h"
#include <iostream>

using namespace std;

enum message_tags {msg_data_packet, msg_broadcast_data, msg_finished};

Client::Client(int clientNumber,mpi::communicator& comm,boost::mutex& mpiMutex) : 
	clientNr(clientNumber), world(comm), hasTask(false),mpiServiceMutex(mpiMutex)
{
	results.reset(new std::vector<float>());

}

void Client::manageWork()
{
	while(1)
	{
		{
			boost::mutex::scoped_lock lock(io_mutex);
			while(!hasTask)
			{
				taskAdded.wait(lock);
			}
		}

	
		{
			boost::mutex::scoped_lock mpiLock(mpiServiceMutex);                
	       	
	        boost::optional<mpi::status> l_status = world.iprobe(clientNr, clientNr) ;
			if(l_status.is_initialized())
			{
				// cout<<"probe"<<endl;
	            world.recv(  l_status->source(), clientNr, *(results.get()));
				hasTask = false;
				mpiLock.unlock();
		     
	   	 	}
	   	 	else
	   	 	{	
   	 			// cout<<"probe else"<<endl;
	   	 		mpiLock.unlock();
		       	boost::this_thread::yield();   
	        	continue;	 		
	   	 	}
	   	 }
   	 	
		bool isFrameComplete = task->savePart(results,task->getPartNumber());
		// cout<<"isFrameComplete: "<<isFrameComplete<<"on thread: "<<boost::this_thread::get_id()<<endl;
		callback(isFrameComplete);
			

		
		
	}
}

void Client::render(boost::shared_ptr<Part> task, boost::function<void (bool)> callback)
{
	boost::mutex::scoped_lock lock(io_mutex);
	// cout<<"Render"<<endl;
	this->task = task;
	this->callback = callback;
	hasTask = true;
	taskAdded.notify_all();
}

int Client::getNumber()
{
	boost::mutex::scoped_lock lock(io_mutex);
	return clientNr;
}