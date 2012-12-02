#include "SqlReader.hpp"
#include <iostream>
#include <sstream>
#include <math.h>
#include <mysql/mysql.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "Task.hpp"
#include "SceneReader.hpp"
using namespace std;
using namespace mysqlpp;
SqlReader::SqlReader():   db("raytracing"),server("127.0.0.1:3306"),user("mpi12345"),pass("aqg2sgpn"),conn(false) {

}

boost::optional<Task> SqlReader::getTask(){
	boost::mutex::scoped_lock lock(io_mutex);
	boost::optional<Task> task;
	if(tasks.size()>0)
	{
		task = tasks.front();
		tasks.pop();
	}		
	return task;
}


void SqlReader::notify(Task task){
	boost::mutex::scoped_lock lock(io_mutex);
	tasks.push(task);
}

void SqlReader::manageWork(){
	
	cout<<"Notyfing!!!!!!!!!"<<endl;	


	ofstream logFile;
	logFile.open("scenaGen.xml");
	logFile<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	logFile<<"<scene xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
	logFile<<"xsi:noNamespaceSchemaLocation=\"scene.xsd\" frames=\"1\" width=\"1920\" ";
	logFile<<"height=\"1080\" w=\"19.2\" h=\"10.8\" d=\"17.0\" bred=\"0.0\" bgreen=\"0.0\" bblue=";
	logFile<<"\"0.0\" glred=\"0.1\" glgreen=\"0.1\" glblue=\"0.1\" >";
	logFile<<"<camera id=\"camera1\" x=\"0\" y=\"0\" z=\"30.0\" vx1=\"1.0\" vx2=\"0.0\" vx3=\"0.0\" vy1=\"0.0\" vy2=\"1.0\" vy3=\"0.0\" vz1=\"0.1\" vz2=\"0.1\" vz3=\"1.0\" />";
	logFile<<"<light id=\"light1\" x=\"110.0\" y=\"0.0\" z=\"300.0\" r=\"0.1\" ared=\"0.2\" agreen=\"0.2\" ablue=\"0.2\" dred=\"0.2\" dgreen=\"0.2\" dblue=\"0.2\" sred=\"0.2\" sgreen=\"0.2\" sblue=\"0.2\" />";
	logFile<<"<light id=\"light1\" x=\"100.0\" y=\"70.0\" z=\"10.0\" r=\"0.1\" ared=\"0.1\" agreen=\"0.1\" ablue=\"0.1\" dred=\"0.1\" dgreen=\"0.1\" dblue=\"0.1\" sred=\"0.1\" sgreen=\"0.1\" sblue=\"0.1\" />";
	
	logFile<<"<sphere id=\"sphere\" ";
	logFile<<"r=\""<<8.0<<"\" ";
	logFile<<"x=\""<<0.0<<"\" ";
	logFile<<"y=\""<<0.0<<"\" ";
	logFile<<"z=\""<<-15.0<<"\" ";
	logFile<<"ared=\""<<0.0<<"\" ";
	logFile<<"ablue=\""<<0.9<<"\" ";
	logFile<<"agreen=\""<<0.0<<"\" ";
	logFile<<"dred=\""<<0.0<<"\" ";
	logFile<<"dblue=\""<<0.0<<"\" ";
	logFile<<"dgreen=\""<<0.09<<"\" ";
	logFile<<"sred=\""<<0.09<<"\" ";
	logFile<<"sblue=\""<<0.9<<"\" ";
	logFile<<"sgreen=\""<<0.09<<"\" ";
	logFile<<"n=\""<<0.3<<"\" ";
	logFile<<"phongN=\""<<0.99<<"\" ";
	logFile<<"blinnN=\""<<10000000.0<<"\" ";
	logFile<<">";
	logFile<<"</sphere>";

	// logFile<<"<sphere id=\"sphere\" ";
	// logFile<<"r=\""<<10.0<<"\" ";
	// logFile<<"x=\""<<-15.0<<"\" ";
	// logFile<<"y=\""<<0.0<<"\" ";
	// logFile<<"z=\""<<0.0<<"\" ";
	// logFile<<"ared=\""<<0.0<<"\" ";
	// logFile<<"ablue=\""<<0.0<<"\" ";
	// logFile<<"agreen=\""<<0.99<<"\" ";
	// logFile<<"dred=\""<<0.0<<"\" ";
	// logFile<<"dblue=\""<<0.0<<"\" ";
	// logFile<<"dgreen=\""<<0.8<<"\" ";
	// logFile<<"sred=\""<<0.0<<"\" ";
	// logFile<<"sblue=\""<<0.0<<"\" ";
	// logFile<<"sgreen=\""<<0.8<<"\" ";
	// logFile<<"n=\""<<0.3<<"\" ";
	// logFile<<"phongN=\""<<0.9<<"\" ";
	// logFile<<"blinnN=\""<<10000.0<<"\" ";
	// logFile<<">";
	// logFile<<"</sphere>";

	// logFile<<"<sphere id=\"sphere\" ";
	// logFile<<"r=\""<<6.0<<"\" ";
	// logFile<<"x=\""<<15.0<<"\" ";
	// logFile<<"y=\""<<0.0<<"\" ";
	// logFile<<"z=\""<<0.0<<"\" ";
	// logFile<<"ared=\""<<0.0<<"\" ";
	// logFile<<"ablue=\""<<0.9<<"\" ";
	// logFile<<"agreen=\""<<0.0<<"\" ";
	// logFile<<"dred=\""<<0.0<<"\" ";
	// logFile<<"dblue=\""<<0.0<<"\" ";
	// logFile<<"dgreen=\""<<0.09<<"\" ";
	// logFile<<"sred=\""<<0.09<<"\" ";
	// logFile<<"sblue=\""<<0.9<<"\" ";
	// logFile<<"sgreen=\""<<0.09<<"\" ";
	// logFile<<"n=\""<<0.15<<"\" ";
	// logFile<<"phongN=\""<<0.99<<"\" ";
	// logFile<<"blinnN=\""<<10000000.0<<"\" ";
	// logFile<<">";
	// logFile<<"</sphere>";


	srand ( time(NULL) );
	for(int i=0;i<4000;i++)
	{	
		logFile<<"<sphere id=\"sphere\" ";
		logFile<<"r=\""<<0.1<<"\" ";
		logFile<<"x=\""<<1*0.02*i*cos(2*3.14*0.01*i)<<"\" ";
		logFile<<"y=\""<<1*0.02*i*sin(2*3.14*0.01*i)<<"\" ";
		logFile<<"z=\""<<0.005*i<<"\" ";
		logFile<<"ared=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"ablue=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"agreen=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"dred=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"dblue=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"dgreen=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"sred=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"sblue=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"sgreen=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"n=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"phongN=\""<<float( rand() % 1000 ) / 1000<<"\" ";
		logFile<<"blinnN=\""<<float( rand() % 1000000 ) +1<<"\" ";
		logFile<<">";
		logFile<<"</sphere>";

	}

	logFile<<"</scene>";
	logFile.close();
// #define TEST_T		
#ifdef TEST_T

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

	c.maxRayBounce = 3;

	c.viewportSize[0] = 19.2;
	c.viewportSize[1] = 10.8;
	c.viewportSize[2] = 17.0;

	c.globalLight[0] = 0.1;
	c.globalLight[1] = 0.1;
	c.globalLight[2] = 0.1;

	c.cameraPosition[0] = 0.0;
	c.cameraPosition[1] = 0.0;
	c.cameraPosition[2] = 30.0;

	c.cameraX[0] = 1.0; c.cameraX[1] = 0.0; c.cameraX[2] = 0.0;
	c.cameraY[0] = 0.0; c.cameraY[1] = 1.0; c.cameraY[2] = 0.0;
	c.cameraZ[0] = 0.1; c.cameraZ[1] = 0.1; c.cameraZ[2] = 1.0;


	Context::Sphere s;

	s.r = 1.0;
	s.position[0] = -5.0; s.position[1] = 0.0; s.position[2] = 0.0;
	s.ambient[0] = 0.2; s.ambient[1] = 0.2; s.ambient[2] = 0.8;
	s.diffuse[0] = 0.1; s.diffuse[1] = 0.5; s.diffuse[2] = 1.0;
	s.specular[0] = 0.1; s.specular[1] = 0.3; s.specular[2] = 1.0;
	s.n = 0.9;
	s.phongN = 0.6;
	s.blinnN = 7000.0;		
	c.spheres.push_back(s);

    s.r = 10.0;
	s.position[0] = -15.0; s.position[1] = 0.0; s.position[2] = 0.0;
	s.ambient[0] = 0.0; s.ambient[1] = 0.0; s.ambient[2] = 0.99;
	s.diffuse[0] = 0.0; s.diffuse[1] = 0.0; s.diffuse[2] = 0.8;
	s.specular[0] = 0.0; s.specular[1] = 0.0; s.specular[2] = 0.8;
	s.n = 0.3;
	s.phongN = 0.90;
	s.blinnN = 10000.0;		
	c.spheres.push_back(s);

	s.r = 6.0;
	s.position[0] = 15.0; s.position[1] = 0.0; s.position[2] = 0.0;
	s.ambient[0] = 0.0; s.ambient[1] = 0.9; s.ambient[2] = 0.0;
	s.diffuse[0] = 0.0; s.diffuse[1] = 0.9; s.diffuse[2] = 0.09;
	s.specular[0] = 0.09; s.specular[1] = 0.9; s.specular[2] = 0.09;
	s.n = 0.15;
	s.phongN = 0.99;
	s.blinnN = 10000000.0;		
	c.spheres.push_back(s);

	s.r = 1.0;
	s.position[0] = 5.0; s.position[1] = 0.0; s.position[2] = 0.0;
	s.ambient[0] = 0.2; s.ambient[1] = 0.2; s.ambient[2] = 0.8;
	s.diffuse[0] = 0.1; s.diffuse[1] = 0.5; s.diffuse[2] = 1.0;
	s.specular[0] = 0.1; s.specular[1] = 0.3; s.specular[2] = 1.0;
	s.n = 0.6;
	s.phongN = 0.6;
	s.blinnN = 7000.0;	
	c.spheres.push_back(s);

	s.r = 1.0;
	s.position[0] = 0.0; s.position[1] = -2.0; s.position[2] = 0.0;
	s.ambient[0] = 0.2; s.ambient[1] = 0.2; s.ambient[2] = 0.8;
	s.diffuse[0] = 0.1; s.diffuse[1] = 0.5; s.diffuse[2] = 1.0;
	s.specular[0] = 0.1; s.specular[1] = 0.3; s.specular[2] = 1.0;
	s.n = 0.6;
	s.phongN = 0.6;
	s.blinnN = 7000.0;
	c.spheres.push_back(s);

	s.r = 1.0;
	s.position[0] = 0.0; s.position[1] = 5.0; s.position[2] = 0.0;
	s.ambient[0] = 0.2; s.ambient[1] = 0.2; s.ambient[2] = 0.8;
	s.diffuse[0] = 0.1; s.diffuse[1] = 0.5; s.diffuse[2] = 0.0;
	s.specular[0] = 0.1; s.specular[1] = 0.3; s.specular[2] = 1.0;
	s.n = 0.6;
	s.phongN = 0.6;
	s.blinnN = 7000.0;	
	c.spheres.push_back(s);

	s.r = 1.0;
	s.position[0] = -3.0; s.position[1] = 3.0; s.position[2] = -19.0;
	s.ambient[0] = 0.1; s.ambient[1] = 0.9; s.ambient[2] = 0.1;
	s.diffuse[0] = 0.9; s.diffuse[1] = 0.9; s.diffuse[2] = 0.0;
	s.specular[0] = 0.9; s.specular[1] = 0.9; s.specular[2] = 0.3;
	s.n = 0.3;
	s.phongN = 0.6;
	s.blinnN = 7000.0;
	c.spheres.push_back(s);

	s.r = 1.0;
	s.position[0] = 3.0; s.position[1] = 3.0; s.position[2] = 0.0;
	s.ambient[0] = 0.2; s.ambient[1] = 0.2; s.ambient[2] = 0.8;
	s.diffuse[0] = 0.1; s.diffuse[1] = 0.5; s.diffuse[2] = 0.0;
	s.specular[0] = 0.1; s.specular[1] = 0.3; s.specular[2] = 1.0;
	s.n = 0.6;
	s.phongN = 0.6;
	s.blinnN = 1.0;
	c.spheres.push_back(s);

	s.r = 1.6;
	s.position[0] = 0.0; s.position[1] = 1.0; s.position[2] = -5.0;
	s.ambient[0] = 0.9; s.ambient[1] = 0.2; s.ambient[2] = 0.0;
	s.diffuse[0] = 0.9; s.diffuse[1] = 1.0; s.diffuse[2] = 0.0;
	s.specular[0] = 0.9; s.specular[1] = 1.0; s.specular[2] = 1.0;
	s.n = 0.8;
	s.phongN = 0.6;
	s.blinnN = 0.9;
	c.spheres.push_back(s);
	

	s.r = 0.6;
	s.position[0] = -2.7; s.position[1] = -0.5; s.position[2] = 4.0;
	s.ambient[0] = 0.99; s.ambient[1] = 0.99; s.ambient[2] = 0.0;
	s.diffuse[0] = 0.9; s.diffuse[1] = 0.99; s.diffuse[2] = 0.0;
	s.specular[0] = 0.9; s.specular[1] = 0.99; s.specular[2] = 0.3;
	s.n = 0.6;
	s.phongN = 0.6;
	s.blinnN = 7000.0;
	c.spheres.push_back(s);

	s.r = 0.6;
	s.position[0] = -1.7; s.position[1] = -0.5; s.position[2] = -4.0;
	s.ambient[0] = 0.99; s.ambient[1] = 0.0; s.ambient[2] = 0.0;
	s.diffuse[0] = 0.9; s.diffuse[1] = 0.0; s.diffuse[2] = 0.0;
	s.specular[0] = 0.9; s.specular[1] = 0.0; s.specular[2] = 0.3;
	s.n = 0.6;
	s.phongN = 0.6;
	s.blinnN = 7000.0;
	c.spheres.push_back(s);

	s.r = 0.6;
	s.position[0] = 1.7; s.position[1] = -0.5; s.position[2] = -4.0;
	s.ambient[0] = 0.99; s.ambient[1] = 0.0; s.ambient[2] = 0.0;
	s.diffuse[0] = 0.9; s.diffuse[1] = 0.0; s.diffuse[2] = 0.0;
	s.specular[0] = 0.9; s.specular[1] = 0.0; s.specular[2] = 0.3;
	s.n = 0.6;
	s.phongN = 0.6;
	s.blinnN = 7000.0;
	c.spheres.push_back(s);


	s.r = 0.6;
	s.position[0] = 2.7; s.position[1] = -0.5; s.position[2] = 4.0;
	s.ambient[0] = 0.99; s.ambient[1] = 0.9; s.ambient[2] = 0.0;
	s.diffuse[0] = 0.9; s.diffuse[1] = 0.9; s.diffuse[2] = 0.0;
	s.specular[0] = 0.9; s.specular[1] = 0.9; s.specular[2] = 0.3;
	s.n = 0.6;
	s.phongN = 0.6;
	s.blinnN = 7000.0;
	c.spheres.push_back(s);


	// srand ( time(NULL) );
	// for(int i=0;i<2000;i++)
	// {
	// 	s.r = float( rand() % 1000 ) / 3000;
	// 	s.position[0] = float( rand() % 1000 ) / 50 - 10; s.position[1] =float( rand() % 1000 ) / 50 - 10; s.position[2] = float( rand() % 1000 ) / 50 - 10;
	// 	s.ambient[0] = float( rand() % 1000 ) / 1000; s.ambient[1] = float( rand() % 1000 ) / 1000; s.ambient[2] = float( rand() % 1000 ) / 1000;
	// 	s.diffuse[0] = float( rand() % 1000 ) / 1000; s.diffuse[1] = float( rand() % 1000 ) / 1000; s.diffuse[2] = float( rand() % 1000 ) / 1000;
	// 	s.specular[0] = float( rand() % 1000 ) / 1000; s.specular[1] = float( rand() % 1000 ) / 1000; s.specular[2] = float( rand() % 1000 ) / 1000;
	// 	s.n = float( rand() % 1000 ) / 1000;
	// 	s.phongN = float( rand() % 1000 )/1000;
	// 	s.blinnN = float( rand() % 1000 ) +1;
	// 	c.spheres.push_back(s);
	// }




///////////////////////////
	
	Context::Light l;
	l.r = 0.1;
	l.position[0] = 110.0; l.position[1] =0.0; l.position[2] = 300;
	l.ambient[0] = 0.2; l.ambient[1] = 0.2; l.ambient[2] = 0.2;
	l.diffuse[0] = 0.2; l.diffuse[1] = 0.2; l.diffuse[2] = 0.2;
	l.specular[0] = 0.2; l.specular[1] = 0.2; l.specular[2] = 0.2;
	c.lights.push_back(l);

	l.r = 0.1;
	l.position[0] = 100.0; l.position[1] = 70.0; l.position[2] = 10;
	l.ambient[0] = 0.1; l.ambient[1] = 0.1; l.ambient[2] = 0.1;
	l.diffuse[0] = 0.1; l.diffuse[1] = 0.1; l.diffuse[2] = 0.1;
	l.specular[0] = 0.1; l.specular[1] = 0.1; l.specular[2] = 0.1;
	c.lights.push_back(l);



	Context::Triangle t;
	t.point1[0] = -1.0; t.point1[1] = -1.0; t.point1[2] = 1.0;
	t.point2[0] = 1.0; t.point2[1] = -1.0; t.point2[2] = 1.0;
	t.point3[0] = 1.0; t.point3[1] = 1.0; t.point3[2] = 1.0;
	t.normalVec[0] = 0.0; t.normalVec[1] = 0.0; t.normalVec[2] = 1.0;
	t.ambient[0] = 0.2; t.ambient[1] = 0.8; t.ambient[2] = 0.2;
	t.diffuse[0] = 0.8; t.diffuse[1] = 0.8; t.diffuse[2] = 0.8;
	t.specular[0] = 0.8; t.specular[1] = 0.8; t.specular[2] = 0.8;
	t.n = 0.9;
	t.blinnN = 6.0;
	t.phongN = 0.5;
	c.triangles.push_back(t);
	t.point3[0] = -1.0; t.point3[1] = -1.0; t.point3[2] = 1.0;
	t.point1[0] = 1.0; t.point1[1] = 1.0; t.point1[2] = 1.0;
	t.point2[0] = -1.0; t.point2[1] = 1.0; t.point2[2] = 1.0;
	
	t.normalVec[0] = 0.0; t.normalVec[1] = 0.0; t.normalVec[2] = 1.0;
	t.ambient[0] = 0.2; t.ambient[1] = 0.8; t.ambient[2] = 0.2;
	t.diffuse[0] = 0.8; t.diffuse[1] = 0.8; t.diffuse[2] = 0.8;
	t.specular[0] = 0.8; t.specular[1] = 0.8; t.specular[2] = 0.8;
	t.n = 0.9;
	t.blinnN = 6.0;
	t.phongN = 0.5;
	c.triangles.push_back(t);


	t.point3[0] = -1.0; t.point3[1] = -1.0; t.point3[2] = 1.0;
	t.point1[0] = -1.0; t.point1[1] = 1.0; t.point1[2] = 1.0;
	t.point2[0] = 1.0; t.point2[1] = 1.0; t.point2[2] = -1.0;
	
	t.normalVec[0] = 0.0; t.normalVec[1] = 0.0; t.normalVec[2] = 1.0;
	t.ambient[0] = 0.2; t.ambient[1] = 0.8; t.ambient[2] = 0.2;
	t.diffuse[0] = 0.8; t.diffuse[1] = 0.8; t.diffuse[2] = 0.8;
	t.specular[0] = 0.8; t.specular[1] = 0.8; t.specular[2] = 0.8;
	t.n = 0.9;
	t.blinnN = 6.0;
	t.phongN = 0.5;
	c.triangles.push_back(t);

	t.point3[0] = -1.0; t.point3[1] = -1.0; t.point3[2] = 1.0;
	t.point1[0] = 1.0; t.point1[1] = 1.0; t.point1[2] = -1.0;
	t.point2[0] = 1.0; t.point2[1] = -1.0; t.point2[2] = -1.0;
	
	t.normalVec[0] = 0.0; t.normalVec[1] = 0.0; t.normalVec[2] = 1.0;
	t.ambient[0] = 0.2; t.ambient[1] = 0.8; t.ambient[2] = 0.2;
	t.diffuse[0] = 0.8; t.diffuse[1] = 0.8; t.diffuse[2] = 0.8;
	t.specular[0] = 0.8; t.specular[1] = 0.8; t.specular[2] = 0.8;
	t.n = 0.9;
	t.blinnN = 6.0;
	t.phongN = 0.5;
	c.triangles.push_back(t);

	// t.point3[0] = 0.0; t.point3[1] = 0.0; t.point3[2] = 0.0;
	// t.point2[0] = 6.0; t.point2[1] = 0.0; t.point2[2] = 0.0;
	// t.point1[0] = 0.0; t.point1[1] = 7.0; t.point1[2] = -5.0;
	// t.normalVec[0] = 0.0; t.normalVec[1] = 0.0; t.normalVec[2] = 1.0;
	// t.ambient[0] = 0.9; t.ambient[1] = 0.8; t.ambient[2] = 0.2;
	// t.diffuse[0] = 0.8; t.diffuse[1] = 0.8; t.diffuse[2] = 0.8;
	// t.specular[0] = 0.8; t.specular[1] = 0.8; t.specular[2] = 0.8;
	// t.n = 0.9;
	// t.blinnN = 6.0;
	// t.phongN = 0.5;
	// c.triangles.push_back(t);




	

	Context::Box b;
	b.minVector[0] = 4.0; b.minVector[1] = 0.0; b.minVector[2] = -9.0;
	b.maxVector[0] = 7.0; b.maxVector[1] = 7.0; b.maxVector[2] = -5.0;	
	b.ambient[0] = 0.2; b.ambient[1] = 0.2; b.ambient[2] = 0.2;
	b.diffuse[0] = 1.0; b.diffuse[1] = 1.0; b.diffuse[2] = 1.0;
	b.specular[0] = 1.0; b.specular[1] = 1.0; b.specular[2] = 1.0;
	b.n = 0.9;
	b.phongN = 0.5;
	b.blinnN = 1.0;

	c.boxes.push_back(b);

	Task task;
	// task.contexts.push_back(c);

	for(int i=0;i<1;i++)
	{
		
		 c.boxes[0].minVector[2] -= 0.05; c.boxes[0].maxVector[2] -= 0.05;

		// c.spheres[2].ambient[0] = 1.0; c.spheres[2].ambient[1] = 0.2; c.spheres[2].ambient[2] = 0.2;
		// c.spheres[2].diffuse[0] = 0.5; c.spheres[2].diffuse[1] = 0.5; c.spheres[2].diffuse[2] = 0.5;
		// c.spheres[2].specular[0] = 0.5; c.spheres[2].specular[1] = 0.5; c.spheres[2].specular[2] = 0.5;
		task.contexts.push_back(c);
	}
	//task.contexts.push_back(c);
	// task.contexts.push_back(c);
	// task.contexts.push_back(c);

	// task.contexts.push_back(c);
	// task.contexts.push_back(c);
	// task.contexts.push_back(c);
	// task.contexts.push_back(c);
	notify(task);
	// task.id = 2;
	// notify(task);
#endif

		
	
	if (conn.connect(db.c_str(), server.c_str(), user.c_str(), pass.c_str())) {
		cout<<"Database connection established"<<endl;
		while(1){
			
			

				std::cout<<"Querying database every 5s!"<<std::endl;	
						
				boost::optional<mysqlpp::StoreQueryResult> res = checkForTasks();
				
				if(res){	
					cout<<"SqlReader: New tasks downloaded from db : "<<res->num_rows()<<std::endl;
					getTaskDataFromDb(res.get());			
				}

				
			boost::posix_time::seconds workTime(2);
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
		
			cout<<"Name : "<<res[0]["name"]<<endl;
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
	boost::mutex::scoped_lock lock(io_mutex);	
	while(!conn.connected()){
			conn.connect(db.c_str(), server.c_str(), user.c_str(), pass.c_str());	
	}
	
	std::stringstream stream;
	stream<<"SELECT * FROM task WHERE id = "<<id;
	mysqlpp::Query query = conn.query(stream.str());
	mysqlpp::StoreQueryResult res = query.store();
	query << "update task SET status = "<<status<<" where id = " <<id<< mysqlpp::quote;
	query.execute();
}

void SqlReader::storeFrameNumber(int id, int frames){
	boost::mutex::scoped_lock lock(io_mutex);

	std::stringstream stream;
	stream<<"SELECT * FROM task WHERE id = "<<id;
	mysqlpp::Query query = conn.query(stream.str());
	mysqlpp::StoreQueryResult res = query.store();
	query << "update task SET total_frames = "<<frames<<" where id = " <<id<< mysqlpp::quote;
	query.execute();
}

void SqlReader::incrementDoneFrames(int id, int doneFrames){
	boost::mutex::scoped_lock lock(io_mutex);

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
  	std::streamoff length = img_file.tellg();
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
	boost::mutex::scoped_lock lock(io_mutex);	
	try{	
		while(!conn.connected()){
			conn.connect(db.c_str(), server.c_str(), user.c_str(), pass.c_str());	
		}
		cout<<"Sciezka do pliku video : "<<path<<endl;
		ifstream img_file;
		img_file.open(path.c_str(), img_file.binary|img_file.in);
		cout<<"Good : "<<img_file.good()<< "fail :"<<img_file.fail()<<"bad:"<<img_file.bad()<<"eof:"<<img_file.eof()<<"rdstate:"<<img_file.rdstate()<<endl;
		img_file.seekg (0, ios::end);
	  	std::streamoff length = img_file.tellg();
		std::cout<<"Dlugosc pliku : "<<length<<std::endl;
	  	img_file.seekg (0, ios::beg);
		
		char * buffer;
		buffer = new char [length];	
		img_file.read (buffer,length);
	  	img_file.close();
		std::string img_data(buffer,length);
		mysqlpp::Query query = conn.query();
		query << "INSERT INTO data (id,task_id,last_frame,video) VALUES(NULL,"<<taskId<<",NULL,\"" <<mysqlpp::escape << img_data << "\")";
		// query << "INSERT INTO data \"" << mysqlpp::escape << img_data << "\" WHERE task_id = " << taskId;
		query.execute();	
		
		delete[] buffer;	
	}
    catch (const BadQuery& er) {
        // Handle any query errors
        cerr << "Query error: " << er.what() << endl;
        return ;
    }
    catch (const BadConversion& er) {
        // Handle bad conversions
        cerr << "Conversion error: " << er.what() << endl <<
                "\tretrieved data size: " << er.retrieved <<
                ", actual size: " << er.actual_size << endl;
        return ;
    }
    catch (const Exception& er) {
        // Catch-all for any other MySQL++ exceptions
        cerr << "Error: " << er.what() << endl;
        return ;
    }    
	
}

