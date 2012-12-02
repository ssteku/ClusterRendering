#ifndef _FRAMESMANAGER_H_
#define _FRAMESMANAGER_H_

#include "WorkerObject.hpp"
#include <queue>
#include <boost/mpi.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/optional.hpp>
#include <boost/thread/condition_variable.hpp>
#include "Context.h"
#include <map>

class Task;
class SqlReader;
class Part;
class Frame;
class Client;

namespace mpi = boost::mpi;
//TODO opisac lepiej klase


/**
  * @brief 
  */
class FramesManager : public WorkerObject
{
	unsigned long avTime;
	unsigned long framesDone;
	/**
	  * @brief SqlReader is object resposible for communication with MySql database
	  */
	SqlReader& sqlReader;

	/**
	  * @brief Queue consists divided tasks wrapped with class Part
	  */
	std::queue<boost::shared_ptr<Part> > partQueue;

	/**
	  * @brief world is a context for boost::mpi
	  */
	mpi::communicator& world;


	boost::mutex io_mutex; /** io_mutex is used to prevent multi read/write on partQueue */
	boost::mutex clientMutex; /** clientMutex is used to prevent multi read/write on freeClients */
	boost::mutex taskMutex; /** Block reading of new task when there is no need */

	boost::mutex mpiServiceMutex;
	/**
	  * @brief taskService is a thread used to serve parts for clients
	  */
	boost::thread taskService;

	/**
	  * @brief tasks provide undone Frames for given taskId
	  */
	std::map<int,map<int,boost::shared_ptr<Frame> > > tasks;

	/**
	  * Queue of clients which don't have task assigned to them	 
	  */
	std::queue<Client*> freeClients;

	/**
	  * List of all created clients, number of clients is equal to number of mpi salves
	  */
	std::map<int, boost::shared_ptr<Client> > clients;

	bool hasFreeClient; /** Variable containing information if there are free clients */
	bool needNewTask; /** Variable which has information if new task should be taken from database */
	boost::condition_variable clientReleased; /** Condition variable informing that there is at least one free client */
	boost::condition_variable addedPart; /** Condition variable informing that there is at least one part to be rendered */
	boost::condition_variable needTaskCondition; /** Condition variable informing that there is ano task rendered right now */
	const unsigned short piecesPerSlave; /** Variable informing how many pieces of frame is for one client */
	boost::posix_time::ptime mst1; /** Time of frame life, show how much time it took to render frame after it's creation */
	
	/**
	 * Function which contain all activities connected with thread live 
	 */
	virtual void manageWork();

	/**
	  * 
	  */
	void addPartsToQueue(Task& task);

	/**
	  * Function adds new part to queue of waiting to be rendered parts
	  */
	boost::optional<boost::shared_ptr<Part> > getPart();

	/**
	  * Function which serves parts for clients and receives information about part status
	  */
	void serveTasks();

	/**
	  * Function release client from list and add client to freeClient
	  */
	void releaseClient(const int clientNumber, const int taskId, const int frameId, long renderTime);

	std::string createMoviePathName(int taskId); /** Function creates path for saving movie with given taskId */


public:
	FramesManager(mpi::communicator& comm,SqlReader& reader);
	virtual ~FramesManager();


};

#endif //_FRAMESMANAGER_H_