#include "Frame.hpp"
#include "ImageCreator.hpp"
#include <iostream>
#include "assert.h"

using namespace std;

Frame::Frame(const int nr, const int parts,const int dimX,const int dimY,unsigned int partLength) : 
	nr(nr), numberOfParts(parts),leftParts(parts),dimX(dimX),dimY(dimY), partLength(partLength)
{
	results.resize(dimX*dimY*3);
	mst1 = boost::posix_time::microsec_clock::local_time();
	
}


//TODO zastanowic sie jak okreslac te wszystki paramsy durne i jak to sensownie wkladac!!!! 
bool Frame::saveToImage(boost::shared_ptr<std::vector<float> > pixels, const int partNr)
{
	 // cout<<"leftParts : "<<leftParts<<"partNr :"<<partNr<<endl;
	boost::mutex::scoped_lock lock(io_mutex);
	 
	 --leftParts;

	if(partNr+1 != numberOfParts)
	{
		// cout<<"partLength : "<<partLength<<endl;

		// cout<<"Normal part"<<endl;
		assert(partLength == (*(pixels.get())).size() );
		int end = partNr*partLength + partLength;
		int old=0;
		for(int i=partNr*partLength;i<end;++i)
		{
			results[i]=(*(pixels.get()))[old];			
			++old;
			// cout<<results[i]<<" ";
		}

	}
	else
	{
		assert((*(pixels.get())).size()==dimX*dimY*3-partNr*partLength);
		// cout<<"End part size: "<<(*(pixels.get())).size()<<"for size : "<<dimX*dimY*3-partNr*partLength <<endl;
		// cout<<"partLength : "<<partLength<<endl;
		int old=0;
		for(int i=partNr*partLength;i<dimX*dimY*3;++i)
		{
			results[i]=(*(pixels.get()))[old];
			++old;
			// cout<<results[i]<<" ";

		}
	}


	
	// cout<<"Saving partNr: "<<partNr<<endl;
	if(!leftParts)
	{
		// cout<<"Saving image size : "<<results.size()<<endl;
		ImageCreator imgCreator;
		imgCreator.saveToImage( results, dimX,dimY,nr, 0);
		boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::time_duration diff = mst2 - mst1;
		cout<<"Time :"<<diff.total_milliseconds() << endl;
		return true;
	}
	else
	{
		return false;
	}
}