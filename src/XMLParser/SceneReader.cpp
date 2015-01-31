//
//  SceneReader.cpp
//  
//
//  Created by Michal Glenc on 104-04-2012.
//

#include "SceneReader.hpp"

//temporary file reader
using namespace std;

SceneReader::SceneReader(istream &is) {
    try {
        si= scene(is);
    }
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}

void SceneReader::initContext(Context &c) {
    //getting scene data
    initSceneData(c);

    //getting camera data
    initCameraData(c);

    //getting sphere data
    initSphereData(c);

    //getting light data
    initLightData(c);

    initBoxData(c);

    initPlaneData(c);

    initTriangleData(c);
}

int SceneReader::getFrames() {
    return static_cast<int>(si->frames());
}

void SceneReader::initCameraData(Context &c) {
    try {
        c.cameraId = si->camera()[0].id();
        
        //camera position
        c.cameraPosition[0] = si->camera()[0].x();
        c.cameraPosition[1] = si->camera()[0].y();
        c.cameraPosition[2] = si->camera()[0].z();
        
        //camera vx
        c.cameraX[0] = si->camera()[0].vx1();
        c.cameraX[1] = si->camera()[0].vx2();
        c.cameraX[2] = si->camera()[0].vx3();
        
        //camera vy
        c.cameraY[0] = si->camera()[0].vy1();
        c.cameraY[1] = si->camera()[0].vy2();
        c.cameraY[2] = si->camera()[0].vy3();
        
        //camera vz
        c.cameraZ[0] = si->camera()[0].vz1();
        c.cameraZ[1] = si->camera()[0].vz2();
        c.cameraZ[2] = si->camera()[0].vz3();
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}

void SceneReader::initLightData(Context &c) {
    try {
        for(sceneInterface::light_const_iterator l (si->light().begin()); l != si->light().end(); l++) {
            Context::Light light;
            
            light.position[0] = l->x();
            light.position[1] = l->y();
            light.position[2] = l->z();
            
            light.r = l->r();
            
            light.ambient[0] = l->ared();
            light.ambient[1] = l->agreen();
            light.ambient[2] = l->ablue();
            
            light.diffuse[0] = l->dred();
            light.diffuse[1] = l->dgreen();
            light.diffuse[2] = l->dblue();
            
            light.specular[0] = l->sred();
            light.specular[1] = l->sgreen();
            light.specular[2] = l->sblue();
            
            light.id = l->id();
            
            //adding light to vector
            c.lights.push_back(light);
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}

void SceneReader::initSphereData(Context &c) {
    try {
        for(sceneInterface::sphere_const_iterator s (si->sphere().begin()); s != si->sphere().end(); s++) {
            Context::Sphere sphere;
            
            sphere.position[0] = s->x();
            sphere.position[1] = s->y();
            sphere.position[2] = s->z();
            
            sphere.r = s->r();
            
            sphere.ambient[0] = s->ared();
            sphere.ambient[1] = s->agreen();
            sphere.ambient[2] = s->ablue();
            
            sphere.diffuse[0] = s->dred();
            sphere.diffuse[1] = s->dgreen();
            sphere.diffuse[2] = s->dblue();
            
            sphere.specular[0] = s->sred();
            sphere.specular[1] = s->sgreen();
            sphere.specular[2] = s->sblue();
            sphere.n = s->n();
            sphere.phongN = s->phongN();
            sphere.blinnN = s->blinnN();
            
            sphere.id = s->id();
            
            //adding sphere to vector
            c.spheres.push_back(sphere);
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}


void SceneReader::initPlaneData(Context& c)
{

    try {
        for(sceneInterface::plane_const_iterator s (si->plane().begin()); s != si->plane().end(); s++) {
            Context::Plane plane;
            
            plane.normalVector[0] = s->x();
            plane.normalVector[1] = s->y();
            plane.normalVector[2] = s->z();
            
            plane.distance = s->d();
            
            plane.ambient[0] = s->ared();
            plane.ambient[1] = s->agreen();
            plane.ambient[2] = s->ablue();
            
            plane.diffuse[0] = s->dred();
            plane.diffuse[1] = s->dgreen();
            plane.diffuse[2] = s->dblue();
            
            plane.specular[0] = s->sred();
            plane.specular[1] = s->sgreen();
            plane.specular[2] = s->sblue();
            
            plane.n = s->n();
            plane.phongN = s->phongN();
            plane.blinnN = s->blinnN();
            
            plane.id = s->id();
            
            //adding plane to vector
            c.planes.push_back(plane);
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }    

}
void SceneReader::initBoxData(Context& c)
{
    try {
        for(sceneInterface::box_const_iterator s (si->box().begin()); s != si->box().end(); s++) {
            Context::Box box;
            
            box.minVector[0] = s->x();
            box.minVector[1] = s->y();
            box.minVector[2] = s->z();

            box.maxVector[0] = s->maxx();
            box.maxVector[1] = s->maxy();
            box.maxVector[2] = s->maxz();           
            
            
            box.ambient[0] = s->ared();
            box.ambient[1] = s->agreen();
            box.ambient[2] = s->ablue();
            
            box.diffuse[0] = s->dred();
            box.diffuse[1] = s->dgreen();
            box.diffuse[2] = s->dblue();
            
            box.specular[0] = s->sred();
            box.specular[1] = s->sgreen();
            box.specular[2] = s->sblue();
            
            box.n = s->n();
            box.phongN = s->phongN();
            box.blinnN = s->blinnN();
            
            box.id = s->id();
            
            //adding box to vector
            c.boxes.push_back(box);
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    } 
}

void SceneReader::initSceneData(Context &c) {
    try {
        //dimensions in px; width x height
        c.dimension[0] = static_cast<int>(si->width());
        c.dimension[1] = static_cast<int>(si->height());
        c.antyAliasing = si->antyalias();
        cout<<"Antyalias scenereader: "<<c.antyAliasing<<endl;
        //viewportSize width x height x depth
        c.viewportSize[0] = si->w();
        c.viewportSize[1] = si->h();
        c.viewportSize[2] = si->d();
        
        //background color
        c.background[0] = si->bred();
        c.background[1] = si->bgreen();
        c.background[2] = si->bblue();
        
        //global light
        c.globalLight[0] = si->glred();
        c.globalLight[1] = si->glgreen();
        c.globalLight[2] = si->glblue();
        c.maxRayBounce = 3;


    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}

void SceneReader::initTriangleData(Context& c) {
    try {
        for(sceneInterface::triangle_const_iterator s (si->triangle().begin()); s != si->triangle().end(); s++) {
            Context::Triangle triangle;
            
            triangle.point1[0] = s->x();
            triangle.point1[1] = s->y();
            triangle.point1[2] = s->z();
            cout<<"xyz1:"<<s->x()<<" "<<s->y()<<" "<<s->z()<<endl;
            triangle.point2[0] = s->x1();
            triangle.point2[1] = s->y1();
            triangle.point2[2] = s->z1(); 
            cout<<"xyz1:"<<s->x1()<<" "<<s->y1()<<" "<<s->z1()<<endl;

            triangle.point3[0] = s->x2();
            triangle.point3[1] = s->y2();
            triangle.point3[2] = s->z2();     
            cout<<"xyz2:"<<s->x2()<<" "<<s->y2()<<" "<<s->z2()<<endl;

            triangle.normalVec[0] = s->xn();
            triangle.normalVec[1] = s->yn();
            triangle.normalVec[2] = s->zn();        
            
            
            triangle.ambient[0] = s->ared();
            triangle.ambient[1] = s->agreen();
            triangle.ambient[2] = s->ablue();
            
            triangle.diffuse[0] = s->dred();
            triangle.diffuse[1] = s->dgreen();
            triangle.diffuse[2] = s->dblue();
            
            triangle.specular[0] = s->sred();
            triangle.specular[1] = s->sgreen();
            triangle.specular[2] = s->sblue();
            
            triangle.n = s->n();
            triangle.phongN = s->phongN();
            triangle.blinnN = s->blinnN();
            
            triangle.id = s->id();
            
            //adding triangle to vector
            c.triangles.push_back(triangle);
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    } 
}

void SceneReader::passCameraData(Context &c, int frame) {    
    try {
        for(tweenInterface::tween_const_iterator t (si->camera()[0].tween().begin()); t != si->camera()[0].tween().end(); t++) {
            if((string)t->type() == "linear") {
                //does animation take place in current frame?
                if(t->start() <= frame && t->end() >= frame) {
                    //counting current value of property
                    float timePart = float(frame - t->start())/float(t->end() - t->start());
                    float result = float(t->from()) + timePart*float(t->to() - t->from());
                    
                    if((string)t->property() == "x") {
                        c.cameraPosition[0] = result;
                    } else if((string)t->property() == "y") {
                        c.cameraPosition[1] = result;
                    } else if((string)t->property() == "z") {
                        c.cameraPosition[2] = result;
                    } else if((string)t->property() == "vx1") {
                        c.cameraX[0] = result;
                    } else if((string)t->property() == "vx2") {
                        c.cameraX[1] = result;
                    } else if((string)t->property() == "vx3") {
                        c.cameraX[2] = result;
                    } else if((string)t->property() == "vy1") {
                        c.cameraY[0] = result;
                    } else if((string)t->property() == "vy2") {
                        c.cameraY[1] = result;
                    } else if((string)t->property() == "vy3") {
                        c.cameraY[2] = result;
                    } else if((string)t->property() == "vz1") {
                        c.cameraZ[0] = result;
                    } else if((string)t->property() == "vz2") {
                        c.cameraZ[1] = result;
                    } else if((string)t->property() == "vz3") {
                        c.cameraZ[2] = result;
                    }
                }
            }
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}

void SceneReader::passSphereData(Context &c, int frame) {
    try {
        for(sceneInterface::sphere_const_iterator s (si->sphere().begin()); s != si->sphere().end(); s++) {
            for (const auto &elem : s->tween()) {
                for (auto &_it : c.spheres) {
                    if (_it.id == s->id()) {
                        if ((string) elem.type() == "linear") {
                            //does animation take place in current frame?
                            if (elem.start() <= frame && elem.end() >= frame) {
                                //counting current value of property
                                float timePart = float(frame - elem.start()) / float(elem.end() - elem.start());
                                float result = float(elem.from()) + timePart * float(elem.to() - elem.from());

                                if ((string) elem.property() == "x") {
                                    _it.position[0] = result;
                                } else if ((string) elem.property() == "y") {
                                    _it.position[1] = result;
                                } else if ((string) elem.property() == "z") {
                                    _it.position[2] = result;
                                } else if ((string) elem.property() == "r") {
                                    _it.r = result;
                                } else if ((string) elem.property() == "ared") {
                                    _it.ambient[0] = result;
                                } else if ((string) elem.property() == "agreen") {
                                    _it.ambient[1] = result;
                                } else if ((string) elem.property() == "ablue") {
                                    _it.ambient[2] = result;
                                } else if ((string) elem.property() == "dred") {
                                    _it.diffuse[0] = result;
                                } else if ((string) elem.property() == "dgreen") {
                                    _it.diffuse[1] = result;
                                } else if ((string) elem.property() == "dblue") {
                                    _it.diffuse[2] = result;
                                } else if ((string) elem.property() == "sred") {
                                    _it.specular[0] = result;
                                } else if ((string) elem.property() == "sgreen") {
                                    _it.specular[1] = result;
                                } else if ((string) elem.property() == "sblue") {
                                    _it.specular[2] = result;
                                } else if ((string) elem.property() == "n") {
                                    _it.n = result;
                                } else if ((string) elem.property() == "phongN") {
                                    _it.phongN = result;
                                    cout<<endl<<"Phong: "<< result<<endl;

                                } else if ((string) elem.property() == "blinnN") {
                                    _it.blinnN = result;
                                    cout<<endl<<"Blinn: "<< result<<endl;

                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}

void SceneReader::passLightData(Context &c, int frame) {
    try {
        for(sceneInterface::light_const_iterator l (si->light().begin()); l != si->light().end(); l++) {
            for (const auto &elem : l->tween()) {
                for (auto &_it : c.lights) {
                    if (_it.id == l->id()) {
                        if ((string) elem.type() == "linear") {
                            //does animation take place in current frame?
                            if (elem.start() <= frame && elem.end() >= frame) {
                                //counting current value of property
                                float timePart = float(frame - elem.start()) / float(elem.end() - elem.start());
                                float result = float(elem.from()) + timePart * float(elem.to() - elem.from());

                                if ((string) elem.property() == "x") {
                                    _it.position[0] = result;
                                } else if ((string) elem.property() == "y") {
                                    _it.position[1] = result;
                                } else if ((string) elem.property() == "z") {
                                    _it.position[2] = result;
                                } else if ((string) elem.property() == "r") {
                                    _it.r = result;
                                } else if ((string) elem.property() == "ared") {
                                    _it.ambient[0] = result;
                                } else if ((string) elem.property() == "agreen") {
                                    _it.ambient[1] = result;
                                } else if ((string) elem.property() == "ablue") {
                                    _it.ambient[2] = result;
                                } else if ((string) elem.property() == "dred") {
                                    _it.diffuse[0] = result;
                                } else if ((string) elem.property() == "dgreen") {
                                    _it.diffuse[1] = result;
                                } else if ((string) elem.property() == "dblue") {
                                    _it.diffuse[2] = result;
                                } else if ((string) elem.property() == "sred") {
                                    _it.specular[0] = result;
                                } else if ((string) elem.property() == "sgreen") {
                                    _it.specular[1] = result;
                                } else if ((string) elem.property() == "sblue") {
                                    _it.specular[2] = result;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}


void SceneReader::passBoxData(Context& c, int frame) {
   try {
        for(sceneInterface::box_const_iterator b (si->box().begin()); b != si->box().end(); b++) {
            for (const auto &elem : b->tween()) {
                for (auto &_it : c.boxes) {
                    if (_it.id == b->id()) {
                        if ((string) elem.type() == "linear") {
                            //does animation take place in current frame?
                            if (elem.start() <= frame && elem.end() >= frame) {
                                //counting current value of property
                                float timePart = float(frame - elem.start()) / float(elem.end() - elem.start());
                                float result = float(elem.from()) + timePart * float(elem.to() - elem.from());

                                if ((string) elem.property() == "x") {
                                    _it.minVector[0] = result;
                                } else if ((string) elem.property() == "y") {
                                    _it.minVector[1] = result;
                                } else if ((string) elem.property() == "z") {
                                    _it.minVector[2] = result;
                                } else if ((string) elem.property() == "maxx") {
                                    _it.maxVector[0] = result;
                                } else if ((string) elem.property() == "maxy") {
                                    _it.maxVector[1] = result;
                                } else if ((string) elem.property() == "maxz") {
                                    _it.maxVector[2] = result;
                                } else if ((string) elem.property() == "ared") {
                                    _it.ambient[0] = result;
                                } else if ((string) elem.property() == "agreen") {
                                    _it.ambient[1] = result;
                                } else if ((string) elem.property() == "ablue") {
                                    _it.ambient[2] = result;
                                } else if ((string) elem.property() == "dred") {
                                    _it.diffuse[0] = result;
                                } else if ((string) elem.property() == "dgreen") {
                                    _it.diffuse[1] = result;
                                } else if ((string) elem.property() == "dblue") {
                                    _it.diffuse[2] = result;
                                } else if ((string) elem.property() == "sred") {
                                    _it.specular[0] = result;
                                } else if ((string) elem.property() == "sgreen") {
                                    _it.specular[1] = result;
                                } else if ((string) elem.property() == "sblue") {
                                    _it.specular[2] = result;
                                } else if ((string) elem.property() == "n") {
                                    _it.n = result;
                                } else if ((string) elem.property() == "phongN") {
                                    _it.phongN = result;
                                } else if ((string) elem.property() == "blinnN") {
                                    _it.blinnN = result;
                                }

                            }
                        }
                    }
                }
            }
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}


void SceneReader::passTriangleData(Context& c, int frame)
{
   try {
        for(sceneInterface::triangle_const_iterator b (si->triangle().begin()); b != si->triangle().end(); b++) {
            for (const auto &elem : b->tween()) {
                for (auto &_it : c.triangles) {
                    if (_it.id == b->id()) {
                        if ((string) elem.type() == "linear") {
                            //does animation take place in current frame?
                            if (elem.start() <= frame && elem.end() >= frame) {
                                //counting current value of property
                                float timePart = float(frame - elem.start()) / float(elem.end() - elem.start());
                                float result = float(elem.from()) + timePart * float(elem.to() - elem.from());
                                cout<<"Czyta trojkat"<<endl;
                                if ((string) elem.property() == "x") {
                                    _it.point1[0] = result;
                                } else if ((string) elem.property() == "y") {
                                    _it.point1[1] = result;
                                } else if ((string) elem.property() == "z") {
                                    _it.point1[2] = result;
                                } else if ((string) elem.property() == "x1") {
                                    _it.point2[0] = result;
                                } else if ((string) elem.property() == "y1") {
                                    _it.point2[1] = result;
                                } else if ((string) elem.property() == "z1") {
                                    _it.point2[2] = result;
                                } else if ((string) elem.property() == "x2") {
                                    _it.point3[0] = result;
                                } else if ((string) elem.property() == "y2") {
                                    _it.point3[1] = result;
                                } else if ((string) elem.property() == "z2") {
                                    _it.point3[2] = result;
                                } else if ((string) elem.property() == "xn") {
                                    _it.normalVec[0] = result;
                                } else if ((string) elem.property() == "yn") {
                                    _it.normalVec[1] = result;
                                } else if ((string) elem.property() == "zn") {
                                    _it.normalVec[2] = result;
                                } else if ((string) elem.property() == "ared") {
                                    _it.ambient[0] = result;
                                } else if ((string) elem.property() == "agreen") {
                                    _it.ambient[1] = result;
                                } else if ((string) elem.property() == "ablue") {
                                    _it.ambient[2] = result;
                                } else if ((string) elem.property() == "dred") {
                                    _it.diffuse[0] = result;
                                } else if ((string) elem.property() == "dgreen") {
                                    _it.diffuse[1] = result;
                                } else if ((string) elem.property() == "dblue") {
                                    _it.diffuse[2] = result;
                                } else if ((string) elem.property() == "sred") {
                                    _it.specular[0] = result;
                                } else if ((string) elem.property() == "sgreen") {
                                    _it.specular[1] = result;
                                } else if ((string) elem.property() == "sblue") {
                                    _it.specular[2] = result;
                                } else if ((string) elem.property() == "n") {
                                    _it.n = result;
                                } else if ((string) elem.property() == "phongN") {
                                    _it.phongN = result;
                                } else if ((string) elem.property() == "blinnN") {
                                    _it.blinnN = result;
                                }

                            }
                        }
                    }
                }
            }
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}

void SceneReader::passPlaneData(Context &c, int frame) {
    try {
         for(sceneInterface::plane_const_iterator p (si->plane().begin()); p != si->plane().end(); p++) {
             for (const auto &elem : p->tween()) {
                 for (auto &_it : c.planes) {
                     if (_it.id == p->id()) {
                         if ((string) elem.type() == "linear") {
                            //does animation take place in current frame?
                             if (elem.start() <= frame && elem.end() >= frame) {
                                //counting current value of property
                                 float timePart = float(frame - elem.start()) / float(elem.end() - elem.start());
                                 float result = float(elem.from()) + timePart * float(elem.to() - elem.from());

                                 if ((string) elem.property() == "x") {
                                     _it.normalVector[0] = result;
                                 } else if ((string) elem.property() == "y") {
                                     _it.normalVector[1] = result;
                                 } else if ((string) elem.property() == "z") {
                                     _it.normalVector[2] = result;
                                 } else if ((string) elem.property() == "d") {
                                     _it.distance = result;
                                 } else if ((string) elem.property() == "ared") {
                                     _it.ambient[0] = result;
                                 } else if ((string) elem.property() == "agreen") {
                                     _it.ambient[1] = result;
                                 } else if ((string) elem.property() == "ablue") {
                                     _it.ambient[2] = result;
                                 } else if ((string) elem.property() == "dred") {
                                     _it.diffuse[0] = result;
                                 } else if ((string) elem.property() == "dgreen") {
                                     _it.diffuse[1] = result;
                                 } else if ((string) elem.property() == "dblue") {
                                     _it.diffuse[2] = result;
                                 } else if ((string) elem.property() == "sred") {
                                     _it.specular[0] = result;
                                 } else if ((string) elem.property() == "sgreen") {
                                     _it.specular[1] = result;
                                 } else if ((string) elem.property() == "sblue") {
                                     _it.specular[2] = result;
                                 } else if ((string) elem.property() == "n") {
                                     _it.n = result;
                                 } else if ((string) elem.property() == "phongN") {
                                     _it.phongN = result;
                                 } else if ((string) elem.property() == "blinnN") {
                                     _it.blinnN = result;
                                }

                            }
                        }
                    }
                }
            }
        }
    }
    
    catch(const xml_schema::exception& e) {
        cerr << e << endl;
    }
}

Context SceneReader::getFrameContext(Context &c, int frame) {
    //putting camera data into c
    c.frameId = frame;
    passCameraData(c, frame);

    //getting sphereData
    passSphereData(c, frame);

    //getting lightData
    passLightData(c, frame);

    passPlaneData(c, frame);

    passBoxData(c, frame);

    passTriangleData(c,frame);

    cout << "Frame #" << frame << endl;
    cout << "Camera\t\t";
    cout << c.cameraPosition[0] << "\t" << c.cameraPosition[1] << "\t" << c.cameraPosition[2] << endl;


    return c;
}

