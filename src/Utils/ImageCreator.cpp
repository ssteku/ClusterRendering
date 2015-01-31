#include "ImageCreator.hpp"
#include "assert.h"
#include <sstream>
#include <sys/stat.h>
#include <png++/png.hpp>

using namespace std;

ImageCreator::ImageCreator() : taskDirectory_("/home/ssteku/input/images/"), pngFileExtension_(".png")
{

}
ImageCreator::~ImageCreator()
{

}

string ImageCreator::saveToImage(vector<char>& pixels, const int dimX,const int dimY, int imageNumber, int taskId)
{
	assert(imageNumber>=0);
	assert(imageNumber<=999);
    png::image<png::rgb_pixel> image(dimX, dimY);
	long cI = 0;
    for (int y = 0; y < dimX; ++y) {
        for (int x = 0; x < dimY; ++x) {
            unsigned char r = pixels[cI];
            unsigned char g = pixels[cI + 1];
            unsigned char b = pixels[cI + 2];
            image[y][x] = png::rgb_pixel(r, g, b);
            cI += 3;
        }
    }

    const std::string imageDirectory = createTaskDirectoryName(taskId);
    createDirectories(imageDirectory);
    const std::string imageFileName = createImageFileName(imageDirectory, imageNumber);

    image.write(imageFileName.c_str());
    return imageFileName;
}

std::string ImageCreator::createTaskDirectoryName(int taskId) {
    stringstream paths;
    paths << taskDirectory_ << taskId << "/";
    return paths.str();
}

void ImageCreator::createDirectories(const std::string &imageDirectory) {
    mkdir(taskDirectory_.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    mkdir(imageDirectory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

std::string ImageCreator::createImageFileName(const std::string &imageDirectory, int imageNumber) {
    stringstream stream;
    stream << imageDirectory;
    if (imageNumber < 10) {
        stream << "image-00" << imageNumber << pngFileExtension_;
    }
    else {
        if (imageNumber >= 10 && imageNumber <= 99) {
            stream << "image-0" << imageNumber << pngFileExtension_;
        }
        else {
            stream << "image-" << imageNumber << pngFileExtension_;
        }
    }
    return stream.str();
}