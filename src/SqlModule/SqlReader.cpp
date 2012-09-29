#include "SqlReader.hpp"
#include <iostream>
#include "Context.h"
#include "Task.hpp"
#include <sstream>
#include <math.h>
#include <mysql/mysql.h>
#include <fstream>

#include "SceneReader.hpp"
using namespace std;
SqlReader::SqlReader():  db("raytracing"),server("127.0.0.1:3306"),user("root"),pass("stek11"),conn(false) {

}

boost::optional<Task> SqlReader::getTask(){
	boost::mutex::scoped_lock lock(io_mutex);
	boost::optional<Task> task;
	if(tasks.size()>0)
	{
		task = tasks.front();
		tasks.pop();
	}		
	lock.unlock();
	return task;
}


void SqlReader::notify(Task task){
	boost::mutex::scoped_lock lock(io_mutex);
	tasks.push(task);
	lock.unlock();
}

void SqlReader::manageWork(){
	cout<<"Notyfing!!!!!!!!!"<<endl;		
	Context c;
	c.dimension[0] = 1920;
	c.dimension[1] = 1080;
	c.window[0][0] = 30;
	c.window[0][1] = 130;
	c.window[1][0] = 230;
	c.window[1][1] = 250;

	c.background[0] = 0.0;
	c.background[1] = 0.0;
	c.background[2] = 0.0;

	c.maxRayBounce = 2;

	c.viewportSize[0] = 19.2;
	c.viewportSize[1] = 10.8;
	c.viewportSize[2] = 17.0;

	c.globalLight[0] = 0.1;
	c.globalLight[1] = 0.1;
	c.globalLight[2] = 0.1;

	c.cameraPosition[0] = 0.0;
	c.cameraPosition[1] = 2.0;
	c.cameraPosition[2] = 15.0;

	c.cameraX[0] = 1.0; c.cameraX[1] = 0.0; c.cameraX[2] = 0.0;
	c.cameraY[0] = 0.0; c.cameraY[1] = 1.0; c.cameraY[2] = 0.0;
	c.cameraZ[0] = 0.0; c.cameraZ[1] = 0.0; c.cameraZ[2] = 1.0;

	Context::Sphere s;
	s.r = 100.0;
	s.position[0] = 4.0; s.position[1] = -102.0; s.position[2] = 0.0;
	s.ambient[0] = 0.1; s.ambient[1] = 0.1; s.ambient[2] = 0.1;
	s.diffuse[0] = 0.1; s.diffuse[1] = 0.1; s.diffuse[2] = 0.1;
	s.specular[0] = 0.9; s.specular[1] = 0.9; s.specular[2] = 0.9;
	s.n = 0;
	c.spheres.push_back(s);

	s.r = 100.0;
	s.position[0] = -108.0; s.position[1] = 0.0; s.position[2] = 0.0;
	s.ambient[0] = 0.1; s.ambient[1] = 0.1; s.ambient[2] = 0.1;
	s.diffuse[0] = 0.1; s.diffuse[1] = 0.1; s.diffuse[2] = 0.1;
	s.specular[0] = 0.9; s.specular[1] = 0.9; s.specular[2] = 0.9;
	s.n = 0;
	c.spheres.push_back(s);

	s.r = 100.0;
	s.position[0] = 0.0; s.position[1] = 0.0; s.position[2] = -110.0;
	s.ambient[0] = 0.1; s.ambient[1] = 0.1; s.ambient[2] = 0.1;
	s.diffuse[0] = 0.1; s.diffuse[1] = 0.1; s.diffuse[2] = 0.1;
	s.specular[0] = 0.9; s.specular[1] = 0.9; s.specular[2] = 0.9;
	s.n = 0;
	c.spheres.push_back(s);

	

	s.r = 2.0;
	s.position[0] = -3.0; s.position[1] = 1.0; s.position[2] = 0.0;
	s.ambient[0] = 0.2; s.ambient[1] = 0.2; s.ambient[2] = 0.2;
	s.diffuse[0] = 0.5; s.diffuse[1] = 0.5; s.diffuse[2] = 0.5;
	s.specular[0] = 0.9; s.specular[1] = 0.9; s.specular[2] = 0.9;
	s.n = 50;
	c.spheres.push_back(s);
	


	s.r = 0.7;
	s.position[0] = 3.5; s.position[1] = 3; s.position[2] = 0.0;
	s.ambient[0] = 0.2; s.ambient[1] = 0.2; s.ambient[2] = 0.2;
	s.diffuse[0] = 0.5; s.diffuse[1] = 0.5; s.diffuse[2] = 0.5;
	s.specular[0] = 0.9; s.specular[1] = 0.9; s.specular[2] = 0.9;
	c.spheres[4].n = 50;
	c.spheres.push_back(s);

	s.r = 1.4;
	s.position[0] = 6; s.position[1] = 3; s.position[2] = 0.0;
	s.ambient[0] = 0.2; s.ambient[1] = 0.2; s.ambient[2] = 0.9;
	s.diffuse[0] = 0.5; s.diffuse[1] = 0.5; s.diffuse[2] = 0.9;
	s.specular[0] = 0.9; s.specular[1] = 0.9; s.specular[2] = 0.9;
	c.spheres[4].n = 50;
	c.spheres.push_back(s);


///////////////////////////
	
	Context::Light l;
	l.r = 10;
	l.position[0] = 0.0; l.position[1] = 0.0; l.position[2] = 32;
	l.ambient[0] = 0.1; l.ambient[1] = 0.1; l.ambient[2] = 0.1;
	l.diffuse[0] = 0.5; l.diffuse[1] = 0.5; l.diffuse[2] = 0.5;
	l.specular[0] = 0.5; l.specular[1] = 0.5; l.specular[2] = 0.5;
	c.lights.push_back(l);


	

	Task task;

	for(int i=0;i<3;i++)
	{
		
		 c.lights[0].position[0] = 0.0; c.lights[0].position[1] = 0.0+0.25*i; c.lights[0].position[2] = 32- 0.1*i;
		l.ambient[0] = 0.1; l.ambient[1] = 0.1+0.01*i; l.ambient[2] = 0.1;

		c.spheres[4].r = 0.7;
		c.spheres[4].position[0] = 3.5-(0.0002*i)*(0.1*i); c.spheres[4].position[1] = 3+i*0.001; c.spheres[4].position[2] = 0.0;
		c.spheres[4].ambient[0] = 1.0; c.spheres[4].ambient[1] = 0.2; c.spheres[4].ambient[2] = 0.2;
		c.spheres[4].diffuse[0] = 0.5; c.spheres[4].diffuse[1] = 0.5; c.spheres[4].diffuse[2] = 0.5;
		c.spheres[4].specular[0] = 0.9; c.spheres[4].specular[1] = 0.9; c.spheres[4].specular[2] = 0.9;
		c.spheres[4].n = 1/(i+1);
		task.contexts.push_back(c);
	}
	// task.contexts.push_back(c);
	// task.contexts.push_back(c);
	// task.contexts.push_back(c);

	// task.contexts.push_back(c);
	// task.contexts.push_back(c);
	// task.contexts.push_back(c);
	// task.contexts.push_back(c);
	notify(task);

	
	if (conn.connect(db.c_str(), server.c_str(), user.c_str(), pass.c_str())) {
		while(1){
			
			std::cout<<"Zapytuje baze co 5s!"<<std::endl;	
					
			boost::optional<mysqlpp::StoreQueryResult> res = checkForTasks();
			
			if(res){	
				cout<<"Pobrano : "<<res->num_rows()<<std::endl;
				getTaskDataFromDb(res.get());			
			}


			boost::posix_time::seconds workTime(1000000);
			boost::this_thread::sleep(workTime);
			
		
		}
	}
	else{
		std::cout<<"Database connection error";
	}
}

void SqlReader::startBehavior(){
	(static_cast<WorkerObject*>(this))->startBehavior();

}

bool  SqlReader::getTaskDataFromDb(mysqlpp::StoreQueryResult& res){
	bool gotTask=false;
	if(1 == res.num_rows()){
		gotTask = true;					
		mysqlpp::DateTime tmpDate = res[0]["created_at"];				
		if(tmpDate.compare(currentTaskDate)>0){
		
			cout<<"Nazwa : "<<res[0]["name"]<<endl;
			cout<<"Context : "<<res[0]["scene_description"]<<endl;
			cout<<"DAta : "<<res[0]["created_at"]<<endl;			
			currentTaskDate = tmpDate;
			std::stringstream contextStream;
			contextStream<<res[0]["scene_description"];
			SceneReader sr(contextStream);  
			Context tmpContext;
			Task task;
			sr.initContext(tmpContext);
			for(int j=0;j<sr.getFrames();j++){
				sr.getFrameContext(tmpContext, j);
				tmpContext.maxRayBounce = 4;
				task.contexts.push_back(tmpContext);
			}	
			task.id = res[0]["id"];						
			
			task.subtaskWidth = res[0]["subtask_width"];
			task.subtaskHeight = res[0]["subtask_height"];
			task.status = res[0]["status"];					
			task.totalFrames = sr.getFrames();
			storeFrameNumber(task.id,task.totalFrames);
			changeStatus(task.id,1);
			notify(task);	
		}

	}
	return gotTask;
}

void SqlReader::changeStatus(int id,int status){
	std::stringstream stream;
	stream<<"SELECT * FROM task WHERE id = "<<id;
	mysqlpp::Query query = conn.query(stream.str());
	mysqlpp::StoreQueryResult res = query.store();
	query << "update task SET status = "<<status<<" where id = " <<id<< mysqlpp::quote;
	query.execute();
}

void SqlReader::storeFrameNumber(int id, int frames){
	std::stringstream stream;
	stream<<"SELECT * FROM task WHERE id = "<<id;
	mysqlpp::Query query = conn.query(stream.str());
	mysqlpp::StoreQueryResult res = query.store();
	query << "update task SET total_frames = "<<frames<<" where id = " <<id<< mysqlpp::quote;
	query.execute();
}

void SqlReader::incrementDoneFrames(int id, int doneFrames){
	std::stringstream stream;
	stream<<"SELECT * FROM task WHERE id = "<<id;
	mysqlpp::Query query = conn.query(stream.str());
	mysqlpp::StoreQueryResult res = query.store();
	query << "update task SET done_frames = "<<doneFrames+1<<" where id = " <<id<< mysqlpp::quote;
	query.execute();
}

boost::optional<mysqlpp::StoreQueryResult> SqlReader::checkForTasks(){
	const std::string queryString="SELECT * FROM task WHERE status = 0 ORDER BY created_at ASC LIMIT 1";  	
	mysqlpp::Query query = conn.query(queryString.c_str());
	mysqlpp::StoreQueryResult res = query.store();
	boost::optional<mysqlpp::StoreQueryResult> retValue;
	if(!res){	
		std::cerr << "Failed to get stock table: " << query.error() << endl;	
		conn.connect(db.c_str(), server.c_str(), user.c_str(), pass.c_str());	
		return boost::optional<mysqlpp::StoreQueryResult>();
	}
	else{
		return boost::optional<mysqlpp::StoreQueryResult>(res);
	}
	
	
	
	
}

void SqlReader::saveImageToDb(int taskId,std::string path){
	if(!conn.connected()){
		conn.connect(db.c_str(), server.c_str(), user.c_str(), pass.c_str());	
	}
	cout<<"Zapis obrzka do bazy!, sciezka: "<<path<<endl;
	ifstream img_file(path.c_str(), ios::ate);
	img_file.seekg (0, ios::end);
  	int length = img_file.tellg();
	cout<<"Dlugosc pliku : "<<length<<endl;
  	img_file.seekg (0, ios::beg);
	
	char * buffer;
	buffer = new char [length];	
	img_file.read (buffer,length);
  	img_file.close();
	std::string img_data(buffer,length);
	mysqlpp::Query query = conn.query();
	query << "DELETE FROM data WHERE task_id="<<taskId;
	query.execute();
	query << "INSERT INTO data (id,task_id,last_frame,video) VALUES(NULL,"<<taskId<<",\"" <<mysqlpp::escape << img_data << "\",NULL)";
	query.execute();	
	
	
	delete[] buffer;	
	

}

void SqlReader::saveVideoToDb(int taskId,std::string path){
	if(!conn.connected()){
		conn.connect(db.c_str(), server.c_str(), user.c_str(), pass.c_str());	
	}
	cout<<"Sciezka do pliku video : "<<path<<endl;
	ifstream img_file;
	img_file.open(path.c_str(), img_file.binary|img_file.in);
	cout<<"Good : "<<img_file.good()<< "fail :"<<img_file.fail()<<"bad:"<<img_file.bad()<<"eof:"<<img_file.eof()<<"rdstate:"<<img_file.rdstate()<<endl;
	img_file.seekg (0, ios::end);
  	int length = img_file.tellg();
	std::cout<<"Dlugosc pliku : "<<length<<std::endl;
  	img_file.seekg (0, ios::beg);
	
	char * buffer;
	buffer = new char [length];	
	img_file.read (buffer,length);
  	img_file.close();
	std::string img_data(buffer,length);
	mysqlpp::Query query = conn.query();
	//query << "INSERT INTO data (id,task_id,last_frame,video) VALUES(NULL,"<<taskId<<",NULL,\"" <<mysqlpp::escape << img_data << "\")";
	query << "UPDATE data SET video = \"" << mysqlpp::escape << img_data << "\" WHERE task_id = " << taskId;
	query.execute();	
	
	delete[] buffer;	
	

}

