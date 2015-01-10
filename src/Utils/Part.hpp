#ifndef _PART_H_
#define _PART_H_

#include <memory>
#include <boost/function.hpp>
#include <mutex>
#include <vector>

class Context;

class Part 
{
	std::shared_ptr<Context> context;
	boost::function<long (std::shared_ptr<std::vector<char> > pixels,const int partNr)> save;
	const unsigned int taskId;
	const unsigned int partNumber;
	const unsigned int frameNr;
	std::mutex io_mutex;
	

public:

	Part(std::shared_ptr<Context> cont, boost::function<long (std::shared_ptr<std::vector<char> > pixels,
		const int partNr)> s, unsigned int tId,unsigned int partNr,const unsigned int fNr);

	unsigned int getTaskId() const ;
	unsigned int getPartNumber() const ;
	unsigned int getFrameNr() const;
	Context* getContext();	 
	long savePart(std::shared_ptr<std::vector<char> > pixels, const int partNr);

};

#endif //_PART_H_