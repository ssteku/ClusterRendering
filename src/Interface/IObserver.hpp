#ifndef _IOBSERVER_H_
#define _IOBSERVER_H_
class Task;

class IObserver{
public:
	virtual void notify(Task task)=0;
};

#endif //_IOBSERVER_H_
