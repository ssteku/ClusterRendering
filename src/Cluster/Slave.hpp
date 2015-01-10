#ifndef _SLAVE_H_
#define _SLAVE_H_

#include "WorkerObject.hpp"

/**
 * Class representing one slave node which compute scene parts and send them to master
 */
class Slave : public WorkerObject{
private:
	/**
	 * Function receives data from master, render scene part and send results to master 
	 */
	virtual void render();
public:
	Slave();
	
};
#endif //_SLAVE_H_
