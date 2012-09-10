#ifndef _SQLREADER_H_
#define _SQLREADER_H_

#include "WorkerObject.hpp"
#include <string>
#include <mysql++/mysql++.h>
#include <mysql++/datetime.h>
#include <boost/optional.hpp>
#include <boost/thread/mutex.hpp>
#include <queue>
#include <boost/optional.hpp>

class Task;

class SqlReader : public WorkerObject{
private:
	void manageWork();
	boost::optional<mysqlpp::StoreQueryResult> checkForTasks();
	bool getTaskDataFromDb(mysqlpp::StoreQueryResult& res);	
	void storeFrameNumber(int id, int frames);
	virtual void notify(Task task);	

public:
	SqlReader();
	void startBehavior();
	void changeStatus(int id,int status);
	void incrementDoneFrames(int id, int doneFrames);
	void saveImageToDb(int taskId,std::string path);
	void saveVideoToDb(int taskId,std::string path);
	virtual boost::optional<Task> getTask();	
private:

	mysqlpp::DateTime currentTaskDate;
	const std::string db;
	const std::string server;
	const std::string user;
	const std::string pass;
	mysqlpp::Connection conn;

	std::queue<Task> tasks;
	boost::mutex io_mutex;
	
};

#endif //_SQLREADER_H_
