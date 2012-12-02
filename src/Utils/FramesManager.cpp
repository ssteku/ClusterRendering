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
	world(comm), hasFreeClient(false),needNewTask(true),piecesPerSlave(3)
{
	cout<<"Pieces per slave: "<<piecesPerSlave<<endl;
	boost::mutex::scoped_lock lock2(clientMutex);
	for(int i=1; i<world.size(); ++i)
	{
		clients[i] = boost::shared_ptr<Client>(new Client(i,world,mpiServiceMutex));
		freeClients.push(clients[i].get());
		clients[i]->startBehavior();
	}
	hasFreeClient = true;
	clientReleased.notify_all();
	avTime = 0;
	framesDone = 0;
}

FramesManager::~FramesManager()
{

}

void FramesManager::manageWork(){
	
	sqlReader.startBehavior();

	taskService = boost::thread(&FramesManager::serveTasks, this);
	while(1)
	{
			{
				boost::mutex::scoped_lock lock(taskMutex);
				while(!needNewTask)		
					needTaskCondition.wait(lock);
			}		
			boost::optional<Task> task = sqlReader.getTask();
			if(task.is_initialized())
			{
				// cout<<"Got task"<<endl;
				addPartsToQueue(task.get());
				sqlReader.changeStatus(task->id,2);

				needNewTask = false;
				
			}	
			else
			{
				executedThread.yield();
				
			}
		}
		boost::posix_time::seconds workTime(1);  
		boost::this_thread::sleep(workTime);		
	
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
		for(unsigned int i=0;i<freeClients.size();++i)
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

				boost::function<void (long)> callback = boost::bind(&FramesManager::releaseClient,this,client->getNumber(),
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



void FramesManager::releaseClient(const int clientNumber, const int taskId, const int frameId, long renderTime)
{
	// cout<<"Client released with removeFrame: "<<removeFrame<<" on thread: "<<boost::this_thread::get_id()<<endl;

	{
		boost::mutex::scoped_lock lock(clientMutex);
		freeClients.push(clients[clientNumber].get());
		hasFreeClient = true;	
		clientReleased.notify_all();
	}

	if(renderTime > 0)
	{
			
			cout<<"Frames manager, frame rendering time : "<<renderTime<<endl;
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
	  			boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::universal_time();
				boost::posix_time::time_duration diff = mst2 - mst1;
				long int timeRender = diff.total_milliseconds();
		  		// cout<<"Time : "<<timeRender<<endl;

		  		

	  			std::map<int,map<int,boost::shared_ptr<Frame> > >::iterator fIt;
	  			fIt = tasks.find(taskId);
				MovieCreator movieCreator;	

				movieCreator.createMovie(taskId);
				sqlReader.saveVideoToDb(taskId,createMoviePathName(taskId));							
				sqlReader.changeStatus(taskId,3);
	  			tasks.erase(fIt);
	  		}
	  		
	}


}

void FramesManager::addPartsToQueue(Task& task)
{
	// unsigned int numberOfParts = piecesPerSlave;
	unsigned int numberOfParts = (world.size()-1)*piecesPerSlave;
	unsigned int partLength = static_cast<unsigned int>(floor(task.contexts[0].dimension[1]/(numberOfParts)));
	// cout<<"Adding task with id: "<<task.id<<endl;
	tasks[task.id] = map<int,boost::shared_ptr<Frame> >();
	

	unsigned int floatsPerPart =  partLength*task.contexts[0].dimension[0]*3;
	for(unsigned int c=0; c<task.contexts.size();++c)
	{
		tasks[task.id][c] = boost::shared_ptr<Frame>(new Frame(c,numberOfParts,task.contexts[c].dimension[0],task.contexts[c].dimension[1],floatsPerPart));
		boost::function<long (boost::shared_ptr<std::vector<float> > pixels, const int partNr)> saver = 
			boost::bind(&Frame::saveToImage, tasks[task.id][c].get(),_1,_2,task.id);
		for(unsigned int i=0;i<numberOfParts;++i){

			boost::shared_ptr<Part> currentContext(new Part(boost::shared_ptr<Context>(new Context(task.contexts[c])),saver, task.id, i,c));

			currentContext->getContext()->window[0][0] = 0;
			currentContext->getContext()->window[1][0] = currentContext->getContext()->dimension[0];	

			currentContext->getContext()->window[0][1]= (i)*partLength;				
			if(i==numberOfParts-1){
				currentContext->getContext()->window[1][1]=currentContext->getContext()->dimension[1];			
			}
			else{
				currentContext->getContext()->window[1][1]= (i+1)*partLength;
			}	
			boost::mutex::scoped_lock lock(io_mutex);			
			partQueue.push(currentContext);
		}
	}
	mst1 = boost::posix_time::microsec_clock::universal_time();
	addedPart.notify_one();


}

boost::optional<boost::shared_ptr<Part> > FramesManager::getPart()
{
	boost::optional<boost::shared_ptr<Part> > retObj;
	
	{
		boost::mutex::scoped_lock lock(io_mutex);			
		if(partQueue.size()>0)
		{
			retObj.reset(partQueue.front());
			partQueue.pop();
		}	
	
	}
	if(partQueue.size()==0)
	{
		boost::mutex::scoped_lock lock(taskMutex);
		needNewTask = true;
		needTaskCondition.notify_all();

	}
	return retObj;
}

std::string FramesManager::createMoviePathName(int taskId)
{
	std::stringstream stream;
	std::string path;	
	stream<<"movies/"<<taskId<<".avi";
	stream>>path;
	return path;
}
