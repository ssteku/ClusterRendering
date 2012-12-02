#include "MovieCreator.hpp"

using namespace std;

MovieCreator::MovieCreator()
{

}
MovieCreator::~MovieCreator()
{

}

void MovieCreator::createMovie(int taskId)
{
	stringstream commStream;	
	commStream<<"avconv -i "<<"images/"<<taskId<<"/image-%03d.png";
	commStream<<" -y";
	commStream<<" -r 30";
	commStream<<" -f flv";
	commStream<<" -b:v 20M";
	commStream<<" movies/"<<taskId<<".avi";
	
	command = 	commStream.str();
	cout<<"Executing command : "<<command<<endl;
	if(0 != system(command.c_str()))
	{
		cout<<"Command system during movie generation returned error code !"<<endl;
	}


}

