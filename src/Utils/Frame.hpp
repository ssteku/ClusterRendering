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
	const int dimX; /** Width of the frame */
	const int dimY; /** Height of the frame */
	unsigned int partLengthInPixels;
	/** Length of result pixel vector (except the last part), used to check if everything was received properly*/

	//TODO WRONG~!~~~~~
	mutable Pixels results;
	Context frameContext;
	Parts parts_;
private:
	Part createPart(unsigned partNr, unsigned partLength);
public:
	Frame(const Context &context);
	/**
	 * Function get vector of pixels and then add it to results vector and if frame is complete, save itself to image
	 */
	void saveToImage(const Pixels &pixels, const int partNr, const int taskId) const;

	void generateImage() const;
};

#endif