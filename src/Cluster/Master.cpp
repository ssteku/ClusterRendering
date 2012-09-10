#include "Master.hpp"
#include "SqlReader.hpp"
#include <vector>
#include <math.h>
#include <sstream>
#include <string>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include "assert.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <sys/stat.h>

using namespace std;

Master::Master(mpi::communicator& comm) : world(comm){
	createDirectoriesTree();
	frameManager.reset(new FramesManager(world,reader));

	frameManager->startBehavior();
}


void Master::createDirectoriesTree(){
	mkdir("images",  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir("movies", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir("logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void Master::manageWork(){
	cout<<"Hello I'm Master"<<endl;

	// cout<<"Frame manager started"<<endl;
	while(1){
		boost::posix_time::seconds workTime(1000000);
		boost::this_thread::sleep(workTime);
	}

}

//TODO wykoasowac
void Master::deployTasks(Task& task,vector<vector<float> >& results){
	// mpi::request reqs[world.size()];
	
	// for(unsigned int c=0; c<task.contexts.size();++c){
	// 	boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
	// 	// Context currentTask = task.contexts[c];
	// 	// int partLength = floor(currentTask.dimension[1]/(world.size()-1));		
	// 	// currentTask.window[0][0] = 0;
	// 	// currentTask.window[1][0] = currentTask.dimension[0];

	// 	// for(int i=1;i<world.size();++i){				
	// 	// 	currentTask.window[0][1]= (i-1)*partLength;				
	// 	// 	if(i==world.size()-1){
	// 	// 		currentTask.window[1][1]=currentTask.dimension[1];			
	// 	// 	}
	// 	// 	else{
	// 	// 		currentTask.window[1][1]= (i)*partLength-1;
	// 	// 	}	
	// 	// 	cout<<"window[1][1] : "<<currentTask.window[1][1]<<endl;	
	// 	// 	reqs[i] = world.isend(i, 0, currentTask);
	// 	// }
	// 	mpi::wait_all(reqs, reqs + world.size());
	// 	cout << "Wyslano contexty do slavow" << "!" << endl;

	// 	for(int i=1;i<world.size();++i){
	// 		reqs[i] = world.irecv(i, i, results[i-1]);
	// 	}
	// 	mpi::wait_all(reqs, reqs + world.size());
	// 	cout << "Otrzymano dane od slavow" << "!" << endl;
	
	// 	vector<float>* pixels = new vector<float>;			
	// 	for(signed int i=1;i<world.size();++i){
	// 		for(unsigned int j=0;j<results[i-1].size();++j){
	// 			pixels->push_back(results[i-1][j]);
	// 		}			
	// 	}
	// 	cout<<"Saving to image"<<endl;
	// 	stringstream stream;
	// 	stream<<task.id;
	// 	// string fileName = imageCreator.saveToImage(pixels,currentTask.dimension,c,task.id);
	// 	cout<<"Sciezka do odczytu dla bazy :"<<fileName<<endl;

	// 	cout<<"Deleting pixels"<<endl;
	// 	delete pixels;
	// 	reader.incrementDoneFrames(task.id, c+1);

	// 	boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
	// 	boost::posix_time::time_duration diff = mst2 - mst1;
	// 	logFile<<task.id<<","<<c<<","<<diff.total_milliseconds() << endl;

	// }
}




