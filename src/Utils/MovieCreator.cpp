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
	system(command.c_str());

	executeVideoConverter();
	

	//avconv -i images/0/image-%03d.png -y -r 20 -f flv movies/0.avi
}


void MovieCreator::manageWork()
{
}

void MovieCreator::executeVideoConverter()
{
}

