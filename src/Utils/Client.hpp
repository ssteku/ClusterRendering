#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <memory>
#include <boost/function.hpp>
#include "WorkerObject.hpp"
#include <mutex>
#include <condition_variable>


class Part;

/**
 * Class represents one mpi client on master client side 
 * object of this class has separate thread which observe and send tasks to mpi client
 * when task was done, client object receive results and pass them to Frame object
 */
class Client : public WorkerObject
{

	const int clientNr; /** Number representing unique client id */
	std::mutex io_mutex;/** Mutex which prevents multi use of render function */
	std::shared_ptr<Part> task;/** */
	boost::function<void (long)> callback;/** Callback function which should be called when part was rendered with frame-finish status*/
	std::condition_variable taskAdded;/** Condition variable which inform about available task */
	bool hasTask;/** Variable informing about availability of task*/
	std::shared_ptr<std::vector<char> > results;/** Vector with pixel colors of rendered scene part */

	/** 
	 * Function which do all activities connected with thread logic,
	 * inside function thread waits for task, then sends task to mpi client if it's possible,
	 * after task was rendered on the slave side, client receives results as a vector of floats
	 */
	virtual void manageWork();

public:
	Client(int clientNumber, std::mutex& mpiMutex);
	/** 
 	 * Function add new task which is part of scene to be rendered remotly and gives callback function which should be called 
 	 * immidiately when task was done.
	 */
	void render(std::shared_ptr<Part> task, boost::function<void (long)> callback);
	/** Function returns unige client id */
	int getNumber() const;
};

#endif //_CLIENT_H_