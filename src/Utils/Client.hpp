#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/mpi.hpp>
#include "WorkerObject.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>


class Part;
namespace mpi = boost::mpi;

/**
 * Class represents one mpi client on master client side 
 * object of this class has separate thread which observe and send tasks to mpi client
 * when task was done, client object receive results and pass them to Frame object
 */
class Client : public WorkerObject
{

	const int clientNr; /** Number representing unique client id */
	mpi::communicator& world;/** MPI environment */
	boost::mutex io_mutex;/** Mutex which prevents multi use of render function */
	boost::mutex& mpiServiceMutex;/** Mutex which prevents multi send to mpi client */
	boost::shared_ptr<Part> task;/** */
	boost::function<void (long)> callback;/** Callback function which should be called when part was rendered with frame-finish status*/
	boost::condition_variable taskAdded;/** Condition variable which inform about available task */
	bool hasTask;/** Variable informing about availability of task*/
	boost::shared_ptr<std::vector<float> > results;/** Vector with pixel colors of rendered scene part */

	/** 
	 * Function which do all activities connected with thread logic,
	 * inside function thread waits for task, then sends task to mpi client if it's possible,
	 * after task was rendered on the slave side, client receives results as a vector of floats
	 */
	virtual void manageWork();

public:
	Client(int clientNumber,mpi::communicator& comm,boost::mutex& mpiMutex);
	/** 
 	 * Function add new task which is part of scene to be rendered remotly and gives callback function which should be called 
 	 * immidiately when task was done.
	 */
	void render(boost::shared_ptr<Part> task, boost::function<void (long)> callback);	
	/** Function returns unige client id */
	int getNumber() const;
};

#endif //_CLIENT_H_