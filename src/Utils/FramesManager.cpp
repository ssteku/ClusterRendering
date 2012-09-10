#include "FramesManager.hpp"
#include "Task.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "SqlReader.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "Part.hpp"
#include "Frame.hpp"
#include "Client.hpp"
#include <iostream>
#include "MovieCreator.hpp"

using namespace std;

FramesManager::FramesManager(mpi::communicator& comm,SqlReader& reader) : sqlReader(reader), 
	world(comm), hasFreeClient(false),hasPart(false),piecesPerSlave(1)
{
	boost::mutex::scoped_lock lock2(clientMutex);
	// cout<<"Frame manager Client"<<endl;
	for(int i=1; i<world.size(); ++i)
	{
		clients[i] = boost::shared_ptr<Client>(new Client(i,world,mpiServiceMutex));
		freeClients.push(clients[i].get());
		clients[i]->startBehavior();
	}
	hasFreeClient = true;
	clientReleased.notify_all();
	// cout<<"world : "<<world.size()<<endl;
	// cout<<"Frame manager after Client"<<endl;

}
FramesManager::~FramesManager()
{

}

void FramesManager::manageWork(){
	
	sqlReader.startBehavior();

	taskService = boost::thread(&FramesManager::serveTasks, this);
	while(1){
				
		boost::optional<Task> task = sqlReader.getTask();
		if(task.is_initialized())
		{
			// cout<<"Got task"<<endl;
			// sqlReader.changeStatus(task->id,2);
			addPartsToQueue(task.get());
			// sqlReader.saveVideoToDb(task.id,path);							
			// sqlReader.changeStatus(task->id,3);
		}	
		else
		{
			executedThread.yield();
			
		}
		
		
		boost::posix_time::seconds workTime(1);  
		boost::this_thread::sleep(workTime);		
	}



	// cout<<"serveTasks 00"<<endl;
	

}

void FramesManager::serveTasks()
{		
	while(1)
	{
		
		{
		
		boost::mutex::scoped_lock lock2(clientMutex);
		while(!hasFreeClient)
			clientReleased.wait(lock2);
		// cout<<"freeClients.size() : "<<freeClients.size()<<endl;
		for(int i=0;i<freeClients.size();++i)
		{
			
			
			boost::optional<boost::shared_ptr<Part> > task = getPart();
			if(task.is_initialized())
			{
				// cout<<"Take free client"<<"on thread: "<<boost::this_thread::get_id()<<endl;
				Client* client = freeClients.front();
				freeClients.pop();
				if(freeClients.size()==0)
				{
					// cout<<"hasFreeClient = false;"<<endl;
					hasFreeClient = false;
				}			

				boost::mutex::scoped_lock mpiL(mpiServiceMutex);

				boost::function<void (bool)> callback = boost::bind(&FramesManager::releaseClient,this,client->getNumber(),
					task.get()->getTaskId(),task.get()->getFrameNr(),_1);
				// cout<<"Taking partNr: "<<task.get()->getPartNumber()<<endl;
				{
				world.send(client->getNumber(), 0, *(task.get()->getContext()));
				}
				client->render(task.get(), callback);
			}
			else
			{
				taskService.yield();				
			}
			
		}
		taskService.yield();
	 	}
	}
}

void FramesManager::waitForFreeClient()
{
	// boost::mutex::scoped_lock lock(clientMutex);
	// cout<<"waitForFreeClient"<<endl;
	
}

void FramesManager::releaseClient(const int clientNumber, const int taskId, const int frameId, bool removeFrame)
{
	// cout<<"Client released with removeFrame: "<<removeFrame<<" on thread: "<<boost::this_thread::get_id()<<endl;

	{
		boost::mutex::scoped_lock lock(clientMutex);
		// cout<<"After mutex"<<endl;
		freeClients.push(clients[clientNumber].get());
		hasFreeClient = true;	
		clientReleased.notify_all();
		// lock.unlock();
	}

	if(removeFrame)
	{
			boost::mutex::scoped_lock lock(io_mutex);
		 	// cout<<"Removing, tasks[taskId].size() : "<<tasks[taskId].size()<<endl;
			map<int,boost::shared_ptr<Frame> >::iterator it;
			it=tasks[taskId].find(frameId);
			(*it).second.reset();
	  		tasks[taskId].erase (it);
	  		//sqlReader.incrementDoneFrames(taskId, c+1);
	  		if(tasks[taskId].size()==0)
	  		{
	  			// cout<<"Removing task"<<endl;
	  			std::map<int,map<int,boost::shared_ptr<Frame> > >::iterator fIt;
	  			fIt = tasks.find(taskId);
				MovieCreator movieCreator;	

				movieCreator.createMovie(taskId);
	  			tasks.erase(fIt);
	  		}
	  		
	}


}

void FramesManager::addPartsToQueue(Task& task)
{
	unsigned int numberOfParts = (world.size()-1)*piecesPerSlave;
	// unsigned int numberOfParts = (1);

	unsigned int partLength = floor(task.contexts[0].dimension[1]/(numberOfParts));

	tasks[task.id] = map<int,boost::shared_ptr<Frame> >();
	

	unsigned int floatsPerPart =  partLength*task.contexts[0].dimension[0]*3;
	for(unsigned int c=0; c<task.contexts.size();++c)
	{
		tasks[task.id][c] = boost::shared_ptr<Frame>(new Frame(c,numberOfParts,task.contexts[c].dimension[0],task.contexts[c].dimension[1],floatsPerPart));
		boost::function<bool (boost::shared_ptr<std::vector<float> > pixels, const int partNr)> saver = 
			boost::bind(&Frame::saveToImage, tasks[task.id][c].get(),_1,_2);
		for(unsigned int i=0;i<numberOfParts;++i){

			boost::shared_ptr<Part> currentContext(new Part(boost::shared_ptr<Context>(new Context(task.contexts[c])),saver, task.id, i,c));

			currentContext->getContext()->window[0][0] = 0;
			currentContext->getContext()->window[1][0] = currentContext->getContext()->dimension[0];	

			currentContext->getContext()->window[0][1]= (i)*partLength;				
			if(i==numberOfParts-1){
				currentContext->getContext()->window[1][1]=currentContext->getContext()->dimension[1];			
				// cout<<"IF"<<endl;
			}
			else{
				currentContext->getContext()->window[1][1]= (i+1)*partLength;
				// cout<<"Else"<<endl;
			}	
			// cout<<"getContext()->window[0][0] : "<<currentContext->getContext()->window[0][0]<<endl;
			// cout<<"getContext()->window[0][1] : "<<currentContext->getContext()->window[0][1]<<endl;	
			// cout<<"getContext()->window[1][0] : "<<currentContext->getContext()->window[1][0]<<endl;
			// cout<<"getContext()->window[1][1] : "<<currentContext->getContext()->window[1][1]<<endl;
			boost::mutex::scoped_lock lock(io_mutex);			
			
			partQueue.push(currentContext);
			// hasPart = true;
		}
	}
	addedPart.notify_one();


}

boost::optional<boost::shared_ptr<Part> > FramesManager::getPart()
{
	boost::mutex::scoped_lock lock(io_mutex);
	boost::optional<boost::shared_ptr<Part> > retObj;
	if(partQueue.size()>0)
	{
		retObj.reset(partQueue.front());
		partQueue.pop();
	}	
	return retObj;
}
