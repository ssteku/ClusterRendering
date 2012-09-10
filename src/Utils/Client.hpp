#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/mpi.hpp>
#include "WorkerObject.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition_variable.hpp>


class Part;
//TODO dodac doxygena
namespace mpi = boost::mpi;

class Client : public WorkerObject
{

	const int clientNr;
	mpi::communicator& world;
	boost::mutex io_mutex;
	boost::mutex& mpiServiceMutex;
	boost::shared_ptr<Part> task;
	boost::function<void (bool)> callback;
	boost::condition_variable taskAdded;
	bool hasTask;
	boost::shared_ptr<std::vector<float> > results;


	virtual void manageWork();

public:
	Client(int clientNumber,mpi::communicator& comm,boost::mutex& mpiMutex);

	void render(boost::shared_ptr<Part> task, boost::function<void (bool)> callback);	
	int getNumber();
};

#endif //_CLIENT_H_