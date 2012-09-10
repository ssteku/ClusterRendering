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


/*
 * @brief 
 */
class FramesManager : public WorkerObject
{
	/*
	 * @brief SqlReader is object resposible for communication with MySql database
	 */
	SqlReader& sqlReader;

	/*
	 * @brief Queue consists divided tasks wrapped with class Part
	 */
	std::queue<boost::shared_ptr<Part> > partQueue;

	/*
	 * @brief world is a context for boost::mpi
	 */
	mpi::communicator& world;

	/*
	 * @brief io_mutex is used to prevent multi read/write on partQueue
	 */
	boost::mutex io_mutex;

	/*
	 * @brief clientMutex is used to prevent multi read/write on freeClients
	 */
	boost::mutex clientMutex;

	boost::mutex mpiServiceMutex;
	/*
	 * @brief taskService is a thread used to serve parts for clients
	 */
	boost::thread taskService;

	/*
	 * @brief tasks provide undone Frames for given taskId
	 */
	std::map<int,map<int,boost::shared_ptr<Frame> > > tasks;

	/*
	 * @brief 
	 */
	std::queue<Client*> freeClients;

	/*
	 * @brief 
	 */
	std::map<int, boost::shared_ptr<Client> > clients;

	bool hasFreeClient;
	bool hasPart;
	boost::condition_variable clientReleased;
	boost::condition_variable addedPart;
	const unsigned short piecesPerSlave;
	/*
	 * @brief 
	 */
	virtual void manageWork();

	/*
	 * @brief 
	 */
	void addPartsToQueue(Task& task);

	/*
	 * @brief 
	 */
	boost::optional<boost::shared_ptr<Part> > getPart();

	/*
	 * @brief 
	 */
	void serveTasks();

	/*
	 * @brief 
	 */
	void releaseClient(const int clientNumber, const int taskId, const int frameId, bool removeFrame);

	void waitForFreeClient();

public:
	/*
	 * @brief 
	 */
	FramesManager(mpi::communicator& comm,SqlReader& reader);
	virtual ~FramesManager();


};

#endif //_FRAMESMANAGER_H_