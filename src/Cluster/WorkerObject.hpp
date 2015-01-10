#ifndef _WORKEROBJECT_H_
#define _WORKEROBJECT_H_

#include <thread>

/**
 * Interface class representing thread 
 */
class WorkerObject{
private:
	virtual void manageWork()=0; /** Function which should do all work during thread life */
public:
	void startBehavior(); /** Function which starts thread */
	void join(); /** Function which wait for thread to be finished */
protected:
	std::thread executedThread; /** Thread object */
};

#endif //_WORKEROBJECT_H_
