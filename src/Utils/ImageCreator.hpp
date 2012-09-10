#ifndef _IMAGECREATOR_H_
#define _IMAGECREATOR_H_
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

class ImageCreator {
private:

public:
ImageCreator();
virtual ~ImageCreator();

virtual std::string saveToImage(std::vector<float>& pixels, const int dimX,const int dimY, int imageNumber, int taskId);

};

#endif //_IMAGECREATOR_H_