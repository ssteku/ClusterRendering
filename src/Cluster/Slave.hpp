#ifndef _SLAVE_H_
#define _SLAVE_H_

#include <boost/mpi.hpp>
#include "WorkerObject.hpp"

namespace mpi = boost::mpi;

/**
 * Class representing one slave node which compute scene parts and send them to master
 */
class Slave : public WorkerObject{
private:
	/**
	 * Function receives data from master, render scene part and send results to master 
	 */
	virtual void manageWork(); 
public:
	Slave(mpi::communicator& comm);
	
private:	
	mpi::communicator& world; /** MPI environment */	
};
#endif //_SLAVE_H_
