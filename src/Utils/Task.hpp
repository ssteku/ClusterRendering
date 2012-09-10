#ifndef _TASK_H_
#define _TASK_H_
#include "Context.h"
#include <string>
#include <vector>


class Task{
public:
	std::vector<Context> contexts;
	unsigned int id;
	unsigned int user_id;
	std::string name,createdAt,modifiedAt;
	unsigned int doneFrames,subtaskWidth,subtaskHeight,status,totalFrames;
};

#endif // _TASK_H_
