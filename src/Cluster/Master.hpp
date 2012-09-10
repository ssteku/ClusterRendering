#ifndef _MASTER_H_
#define _MASTER_H_

#include <iostream>
#include <fstream>
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include <queue>
#include <boost/scoped_ptr.hpp>
#include "Task.hpp"
#include "WorkerObject.hpp"
#include "IObserver.hpp"
#include "SqlReader.hpp"
#include "ImageCreator.hpp"
#include "MovieCreator.hpp"
#include "FramesManager.hpp"
//TODO dodac doxygena
namespace mpi = boost::mpi;

class Master: public WorkerObject{
private:
	ofstream logFile;
	ImageCreator imageCreator; 
	MovieCreator movieCreator;
	SqlReader reader;
	boost::scoped_ptr<FramesManager> frameManager;
	void createDirectoriesTree();
	virtual void manageWork();
	void createAviFilm(int taskId);
	void deployTasks(Task& task,std::vector<std::vector<float> >& results);
public:

	Master(mpi::communicator& comm);	
	
private:
	

	mpi::communicator& world;	
	
	
};

#endif //_MASTER_H_
