#ifndef _FRAME_H_
#define _FRAME_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <boost/thread/mutex.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

 /**
  * Class representing one frame of the animation, it contains all information about one frame and it can save itself to image
  * when is complete
  */
class Frame
{
	const int nr; /** Unique frame id */
	const int numberOfParts; /** Number of parts which frame has to compute */
	int leftParts; /** Number of parts which still hase to be rendered */
	const int dimX; /** Width of the frame */
	const int dimY; /** Height of the frame */
	unsigned int partLength; /** Length of result pixel vector (except the last part), used to check if everything was received properly*/
	std::vector<float> results; /** Vector with rendered, received pixels, which is saved to image when is fill completelly */
	boost::posix_time::ptime mst1; /** Time of frame life, show how much time it took to render frame after it's creation */
	boost::mutex io_mutex; /** Mutex which prevents saving to image by many threads */
	


public:
	Frame(const int nr, const int parts,const int dimX,const int dimY,unsigned int partLength);
	/**
	 * Function get vector of pixels and then add it to results vector and if frame is complete, save itself to image
	 */
	long saveToImage(boost::shared_ptr<std::vector<float> > pixels, const int partNr,const int taskId);
};

#endif