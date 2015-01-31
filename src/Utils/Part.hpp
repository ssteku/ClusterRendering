#ifndef _PART_H_
#define _PART_H_

#include <memory>
#include <boost/function.hpp>
#include <mutex>
#include <vector>
#include "Context.h"
#include "DataDefs.hpp"

class Context;

class Part 
{
	Context context;

public:

	Part(Context cont, unsigned int partNr);

	Pixels calculatePart();
};

#endif //_PART_H_