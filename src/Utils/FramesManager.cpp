#include "FramesManager.hpp"
#include "Task.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "Part.hpp"
#include "Frame.hpp"
#include "Client.hpp"
#include <iostream>
#include "MovieCreator.hpp"
#include <mutex>
using namespace std;

FramesManager::FramesManager() :
		fileManager_("input"), hasFreeClient(false),needNewTask(true),piecesPerSlave(1)
{
	fileManager_.startBehavior();
	cout<<"Pieces per slave: "<<piecesPerSlave<<endl;

	avTime = 0;
	framesDone = 0;
}

FramesManager::~FramesManager()
{

}

void FramesManager::manageWork()
{
	while(1)
	{
		boost::optional<Task> task = fileManager_.getTask();
		if(task)
		{
			cout<<"Got task"<<endl;
			calculateTask(task.get());
		}
		std::chrono::milliseconds dura( 3000 );
		std::this_thread::sleep_for( dura );
	}

}

void FramesManager::calculateTask(Task &task)
{
	// unsigned int numberOfParts = piecesPerSlave;
	const unsigned numberOfThreads = 8;
	unsigned int numberOfParts = (numberOfThreads-1)*piecesPerSlave;
	unsigned int partLength = static_cast<unsigned int>(floor(task.contexts[0].dimension[1]/(numberOfParts)));
	// cout<<"Adding task with id: "<<task.id<<endl;
	tasks[task.id] = map<int,std::shared_ptr<Frame> >();


	unsigned int floatsPerPart =  partLength*task.contexts[0].dimension[0]*3;
	for(unsigned int c=0; c<task.contexts.size();++c)
	{
		tasks[task.id][c] = std::shared_ptr<Frame>(new Frame(c,numberOfParts,task.contexts[c].dimension[0],task.contexts[c].dimension[1],floatsPerPart));
		boost::function<long (std::shared_ptr<std::vector<char> > pixels, const int partNr)> saver =
				boost::bind(&Frame::saveToImage, tasks[task.id][c].get(),_1,_2,task.id);
		for(unsigned int i=0;i<numberOfParts;++i){

			std::shared_ptr<Part> currentContext(new Part(std::shared_ptr<Context>(new Context(task.contexts[c])),saver, task.id, i,c));

			currentContext->getContext()->window[0][0] = 0;
			currentContext->getContext()->window[1][0] = currentContext->getContext()->dimension[0];

			currentContext->getContext()->window[0][1]= (i)*partLength;
			if(i==numberOfParts-1){
				currentContext->getContext()->window[1][1]=currentContext->getContext()->dimension[1];
			}
			else{
				currentContext->getContext()->window[1][1]= (i+1)*partLength;
			}
		}
	}

//	client->render(task.get(), callback);


}


::string FramesManager::createMoviePathName(int taskId)
{
	std::stringstream stream;
	std::string path;
	stream<<"movies/"<<taskId<<".avi";
	stream>>path;
	return path;
}
