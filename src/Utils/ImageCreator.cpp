#include "ImageCreator.hpp"
#include "assert.h"
#include <sstream>
#include <sys/stat.h>
#include <png++/png.hpp>

using namespace std;

ImageCreator::ImageCreator()
{

}
ImageCreator::~ImageCreator()
{

}

string ImageCreator::saveToImage(vector<char>& pixels, const int dimX,const int dimY, int imageNumber, int taskId)
{
	assert(imageNumber>=0);
	assert(imageNumber<=999);
	// cout<<"saveToImage results.size: "<<pixels.size()<<" should be : "<<dimX*dimY<<endl;
	int myDimX = dimX;
	int myDimY = dimY;
	png::image< png::rgb_pixel > image(myDimX, myDimY);
	// cout<<"y: "<<myDimY<<"x: "<<myDimX<<endl;
	long cI = 0;
	for(int y = 0; y < myDimY; ++y) {
		// cout<<"y:"<<y<<endl;
		for(int x = 0; x < myDimX; ++x) {
			// cout<<"x:"<<x<<endl;
			unsigned char r = pixels[cI];
			unsigned char g = pixels[cI+1];
			unsigned char b = pixels[cI+2];
			image[y][x] =  png::rgb_pixel(r, g, b);
			cI += 3;
			// cout<<cI<<endl;
		}
		// cout<<cI<<endl;
	}

	// cout<<"yuppi"<<endl;
	stringstream stream,paths;
	string path;
    paths << "/home/ssteku/input/images/" << taskId << "/";
	paths>>path;
	// cout<<"Path dla obrazka to :"<<path<<endl;
    mkdir("/home/ssteku/input/images", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	if(imageNumber<10){
		stream<<path<<"image-00"<<imageNumber<<".png";
	}
	else{
		if(imageNumber>=10 && imageNumber<=99){
			stream<<path<<"image-0"<<imageNumber <<".png";
		}
		else
		{
			stream<<path<<"image-"<<imageNumber<<".png";
		}	
	}

	string fileName;
	stream>>fileName;
	image.write(fileName.c_str());
	return fileName;	
}