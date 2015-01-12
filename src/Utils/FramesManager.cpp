#include "FramesManager.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "Part.hpp"
#include "Frame.hpp"

using namespace std;

FramesManager::FramesManager() :
		fileManager_("input"), hasFreeClient(false),needNewTask(true),piecesPerSlave(1)
{
	fileManager_.startBehavior();
	cout<<"Pieces per slave: "<<piecesPerSlave<<endl;
}

FramesManager::~FramesManager()
{

}

void FramesManager::manageWork()
{
	while(1)
	{
		cout << "FramesManager::Waiting for tasks" << endl;
		boost::optional<Task> task = fileManager_.getTask();
		if(task)
		{
			cout << "FramesManager::Got task" << endl;
			calculateTask(task.get());
		}
		std::chrono::milliseconds dura( 3000 );
		std::this_thread::sleep_for( dura );
	}

}

Frames FramesManager::getFrames(const Task &task)
{
	Frames taskFrames;
	std::cout << "FramesManager::Frames size : " << task.contexts.size() << std::endl;
	std::transform(std::begin(task.contexts), std::end(task.contexts), std::back_inserter(taskFrames),
			[](const Context &context) {
				return Frame(context);
			});
	return taskFrames;
}

void FramesManager::calculateTask(const Task &task) {
	Frames frames = getFrames(task);
	std::for_each(std::begin(frames), std::end(frames), [](const Frame &frame) {
		frame.generateImage();
	});
}


::string FramesManager::createMoviePathName(int taskId)
{
	std::stringstream stream;
	std::string path;
	stream<<"movies/"<<taskId<<".avi";
	stream>>path;
	return path;
}
