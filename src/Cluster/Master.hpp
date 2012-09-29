#ifndef _MASTER_H_
#define _MASTER_H_

#include <boost/mpi.hpp>
#include <boost/scoped_ptr.hpp>
#include "Task.hpp"
#include "WorkerObject.hpp"
#include "SqlReader.hpp"
#include "FramesManager.hpp"
//TODO dodac doxygena
namespace mpi = boost::mpi;

/**
 * Class representing master node thread, should create directory tree and start serving tasks for slaves
 */
class Master: public WorkerObject{
private:
	mpi::communicator& world;	/** MPI environment */		
	SqlReader reader; /** SQL reader which enable task reading from DB */
	/** Frame manager object which serve tasks for slaves and do all jobs connected with 
	 * collecting results, serving tasks, balancing load	
	 */
	boost::scoped_ptr<FramesManager> frameManager; 
	void createDirectoriesTree(); /** Function creates required directory tree */	
	virtual void manageWork(); /** Function waits until all FrameManager thread is finished*/
public:

	Master(mpi::communicator& comm);	
	
};

#endif //_MASTER_H_
