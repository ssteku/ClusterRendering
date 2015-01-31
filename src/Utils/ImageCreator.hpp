#ifndef _IMAGECREATOR_H_
#define _IMAGECREATOR_H_
#include <vector>
#include <string>
#include <memory>

class ImageCreator {
private:

public:
ImageCreator();
virtual ~ImageCreator();

virtual std::string saveToImage(std::vector<char>& pixels, const int dimX,const int dimY, int imageNumber, int taskId);

private:
    std::string createTaskDirectoryName(int taskId);

    void createDirectories(const std::string &imageDirectory);

    std::string createImageFileName(const std::string &imageDirectory, int imageNumber);

private:
    const std::string taskDirectory_;
    const std::string pngFileExtension_;
};

#endif //_IMAGECREATOR_H_