#include "Part.hpp"
#include "Context.h"
#include <iostream>

using namespace std;

Part::Part(std::shared_ptr<Context> cont, boost::function<long (std::shared_ptr<std::vector<char> > pixels,
		const int partNr)> s, unsigned int tId,unsigned int partNr, const unsigned int fNr) : 
	context(cont), save(s),taskId(tId), partNumber(partNr), frameNr(fNr)
{

}

unsigned int Part::getTaskId() const
{
	return taskId;
}
unsigned int Part::getPartNumber() const
{
	return partNumber;
} 

unsigned int Part::getFrameNr() const
{
	return frameNr;
}

Context* Part::getContext()
{
	return (context.get());
}

long Part::savePart(std::shared_ptr<std::vector<char> > pixels, const int partNr)
{
	std::unique_lock<std::mutex> lock(io_mutex);
	long ret = save(pixels, partNr);
	return ret;
}

