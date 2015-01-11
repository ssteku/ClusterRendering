#ifndef _SLAVE_H_
#define _SLAVE_H_

#include "WorkerObject.hpp"

class Context;
/**
 * Class representing one slave node which compute scene parts and send them to master
 */
class Slave {
private:
	/**
	 * Function receives data from master, render scene part and send results to master 
	 */
	virtual void render(const Context &context);
public:
	Slave();
	
};
#endif //_SLAVE_H_
