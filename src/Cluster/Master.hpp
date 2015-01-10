#ifndef _MASTER_H_
#define _MASTER_H_

#include "Task.hpp"
#include "WorkerObject.hpp"
#include "FramesManager.hpp"

/**
 * Class representing master node thread, should create directory tree and start serving tasks for slaves
 */
class Master: public WorkerObject{
private:
	/** Frame manager object which serve tasks for slaves and do all jobs connected with
	 * collecting results, serving tasks, balancing load	
	 */
	std::unique_ptr<FramesManager> frameManager;
	void createDirectoriesTree(); /** Function creates required directory tree */	
	virtual void manageWork(); /** Function waits until all FrameManager thread is finished*/
public:
	Master();
	
};

#endif //_MASTER_H_
