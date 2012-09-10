#ifndef _WORKEROBJECT_H_
#define _WORKEROBJECT_H_

#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

class WorkerObject{
private:
	virtual void manageWork()=0;
public:
	void startBehavior();
	void join();
protected:
	boost::thread executedThread;
};

#endif //_WORKEROBJECT_H_
