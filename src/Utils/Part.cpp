#include "Part.hpp"
#include "Context.h"

Part::Part(boost::shared_ptr<Context> cont, boost::function<bool (boost::shared_ptr<std::vector<float> > pixels, 
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

bool Part::savePart(boost::shared_ptr<std::vector<float> > pixels, const int partNr)
{
	boost::mutex::scoped_lock lock(io_mutex);
	bool ret = save(pixels, partNr);
	if(ret)
		// cout<<"Part ret: "<<ret<<endl;
	return ret;
}

