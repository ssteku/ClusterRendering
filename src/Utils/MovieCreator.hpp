#ifndef MOVIECREATOR
#define MOVIECREATOR
#include <string>
#include "WorkerObject.hpp"
#include <vector>
#include <sstream>

class MovieCreator 
{
private:
	std::string command;
public:
	MovieCreator();
	~MovieCreator();

	virtual void createMovie(int taskId); 

};

#endif //MOVIECREATOR