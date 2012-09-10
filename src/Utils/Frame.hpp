#ifndef _FRAME_H_
#define _FRAME_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <boost/thread/mutex.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

//TODO dodac doxygena
class Frame
{
	const int nr;
	const int numberOfParts;
	int leftParts;
	const int dimX;
	const int dimY;
	unsigned int partLength;
	std::vector<float> results;
	boost::posix_time::ptime mst1;
	boost::mutex io_mutex;
	


public:
	Frame(const int nr, const int parts,const int dimX,const int dimY,unsigned int partLength);
	bool saveToImage(boost::shared_ptr<std::vector<float> > pixels, const int partNr);
};

#endif