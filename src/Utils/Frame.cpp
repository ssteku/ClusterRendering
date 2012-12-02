#include "Frame.hpp"
#include "ImageCreator.hpp"
#include <iostream>
#include "assert.h"

using namespace std;

Frame::Frame(const int nr, const int parts,const int dimX,const int dimY,unsigned int partLength) : 
	nr(nr), numberOfParts(parts),leftParts(parts),dimX(dimX),dimY(dimY), partLength(partLength)
{
	results.resize(dimX*dimY*3);
	
}


long Frame::saveToImage(boost::shared_ptr<std::vector<float> > pixels, const int partNr,const int taskId)
{
	if(leftParts == numberOfParts)
	{
		mst1 = boost::posix_time::microsec_clock::universal_time();
	}
	
	boost::mutex::scoped_lock lock(io_mutex);
	 
	 --leftParts;

	if(partNr+1 != numberOfParts)
	{
		assert(partLength == (*(pixels.get())).size() );
		int end = partNr*partLength + partLength;
		int old=0;
		for(int i=partNr*partLength;i<end;++i)
		{
			results[i]=(*(pixels.get()))[old];			
			++old;
		}

	}
	else
	{
		assert((*(pixels.get())).size()==dimX*dimY*3-partNr*partLength);
		int old=0;
		for(int i=partNr*partLength;i<dimX*dimY*3;++i)
		{
			results[i]=(*(pixels.get()))[old];
			++old;

		}
	}
	
	if(!leftParts)
	{
		ImageCreator imgCreator;
		imgCreator.saveToImage( results, dimX,dimY,nr, taskId);
		boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::universal_time();
		boost::posix_time::time_duration diff = mst2 - mst1;
		long int timeRender = diff.total_milliseconds();
		return timeRender;
	}
	else
	{
		return 0;
	}
}