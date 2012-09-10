#ifndef _SLAVE_H_
#define _SLAVE_H_

#include <iostream>
#include <boost/thread.hpp>
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include "WorkerObject.hpp"

namespace mpi = boost::mpi;
class Slave : public WorkerObject{
private:
	virtual void manageWork();
public:
	Slave(mpi::communicator& comm);
	
private:
	mpi::communicator& world;	
};
#endif //_SLAVE_H_
