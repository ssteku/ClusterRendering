#ifndef _PART_H_
#define _PART_H_

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>

class Context;

class Part 
{
	boost::shared_ptr<Context> context;
	boost::function<long (boost::shared_ptr<std::vector<float> > pixels,const int partNr)> save;
	const unsigned int taskId;
	const unsigned int partNumber;
	const unsigned int frameNr;
	boost::mutex io_mutex;
	

public:

	Part(boost::shared_ptr<Context> cont, boost::function<long (boost::shared_ptr<std::vector<float> > pixels, 
		const int partNr)> s, unsigned int tId,unsigned int partNr,const unsigned int fNr);

	unsigned int getTaskId() const ;
	unsigned int getPartNumber() const ;
	unsigned int getFrameNr() const;
	Context* getContext();	 
	long savePart(boost::shared_ptr<std::vector<float> > pixels, const int partNr);

};

#endif //_PART_H_