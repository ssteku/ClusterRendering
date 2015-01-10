#ifndef _FRAMESMANAGER_H_
#define _FRAMESMANAGER_H_

#include "WorkerObject.hpp"
#include <queue>
#include <memory>
#include <boost/optional.hpp>
#include <condition_variable>
#include "Context.h"
#include <map>
#include <mutex>
#include "FileManager.hpp"
class Task;
class Part;
class Frame;
class Client;

//TODO opisac lepiej klase


/**
  * @brief 
  */
class FramesManager : public WorkerObject
{
	unsigned long avTime;
	unsigned long framesDone;
	
	/**
	 * @brief object which allows to read tasks from files
	 */
	FileManager fileManager_;

	/**
	  * @brief tasks provide undone Frames for given taskId
	  */
	std::map<int,map<int,std::shared_ptr<Frame> > > tasks;

	bool hasFreeClient; /** Variable containing information if there are free clients */
	bool needNewTask; /** Variable which has information if new task should be taken from database */
	const unsigned short piecesPerSlave; /** Variable informing how many pieces of frame is for one client */

	/**
	 * Function which contain all activities connected with thread live 
	 */
	virtual void manageWork();

	/**
	  * 
	  */
	void calculateTask(Task &task);

	std::string createMoviePathName(int taskId); /** Function creates path for saving movie with given taskId */


public:
	FramesManager();
	virtual ~FramesManager();


};

#endif //_FRAMESMANAGER_H_