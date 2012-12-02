//
//  SceneReader.hpp
//  
//
//  Created by Michał Glenc on 104-04-2012.
//

#ifndef _SceneReader_hpp
#define _SceneReader_hpp

#include <iostream>
#include <sstream>

#include "../Utils/Context.h"
#include "scene.hxx"

using namespace std;

class SceneReader {
private:
    istream &xmlStream;
    
    auto_ptr<sceneInterface> si;
    
public:
    SceneReader(istream & is);
    //SceneReader(string s);
    
    void initContext(Context &c);
    
    int getFrames();
    
    void initSceneData(Context& c);
    void initCameraData(Context& c);
    void initSphereData(Context& c);
    void initPlaneData(Context& c);
    void initBoxData(Context& c);
    void initTriangleData(Context& c);

    void initLightData(Context& c);
    
    void passCameraData(Context &c, int frame);
    void passSphereData(Context &c, int frame);
    void passPlaneData(Context &c, int frame);
    void passBoxData(Context& c, int frame);
    void passTriangleData(Context& c, int frame);
    void passLightData(Context &c, int frame);
    
    Context getFrameContext(Context &c, int frame);
};

#endif
