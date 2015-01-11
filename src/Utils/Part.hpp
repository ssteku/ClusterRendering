#ifndef _PART_H_
#define _PART_H_

#include <memory>
#include <boost/function.hpp>
#include <mutex>
#include <vector>
#include "Context.h"

class Context;

class Part 
{
	Context context;
	const unsigned int partNumber;


public:

	Part(Context cont, unsigned int partNr);

	unsigned int getPartNumber() const ;

	const Context &getContext();

};

#endif //_PART_H_