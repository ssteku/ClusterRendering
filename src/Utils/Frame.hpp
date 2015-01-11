#ifndef _FRAME_H_
#define _FRAME_H_

#include <memory>
#include <vector>
#include <mutex>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "DataDefs.hpp"
#include "Context.h"

/**
  * Class representing one frame of the animation, it contains all information about one frame and it can save itself to image
  * when is complete
  */
class Frame
{
	const int numberOfParts; /** Number of parts which frame has to compute */
	int leftParts; /** Number of parts which still hase to be rendered */
	const int dimX; /** Width of the frame */
	const int dimY; /** Height of the frame */
	unsigned int partLengthInPixels;
	/** Length of result pixel vector (except the last part), used to check if everything was received properly*/
	Pixels results;
	/** Vector with rendered, received pixels, which is saved to image when is fill completelly */
	boost::posix_time::ptime mst1; /** Time of frame life, show how much time it took to render frame after it's creation */
	Context frameContext;
	Parts parts_;

public:
	Frame(const Context &context);
	/**
	 * Function get vector of pixels and then add it to results vector and if frame is complete, save itself to image
	 */
	long saveToImage(std::shared_ptr<Pixels> pixels, const int partNr, const int taskId);
};

#endif