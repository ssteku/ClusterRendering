#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "MaterialObject.hpp"
#include "Light.hpp"
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreColourValue.h>
#include <string>
#include <vector>

using namespace std;

namespace rendering
{

class Scene 
{
public:
	vector<boost::shared_ptr<MaterialObject> > objects;

	vector<boost::shared_ptr<Light> > lights;

	//TODO
	//Powinno być w Ogre!!!
	int dimension[2]; /** Dimension of the result image(scene) */ 
	int window[2][2]; /** Size of the part which has to be computed, describes two points of the rectangle*/ 
	int antyAliasing; /** Value of antyaliasing factor */
	Ogre::Vector3 viewportSize; /** Size of the real system */
	Ogre::ColourValue background; /** Background rgb colour */ 
	Ogre::ColourValue globalLight; /** Ambient global rgb light colour */ 
	
	string cameraId; /** Unique camera id */
	Ogre::Vector3 cameraPosition; /** Position of the camera [x][y][z] */ 
	/** Camera looks at the opposite side then vector specified below */	
	Ogre::Vector3 cameraX; /** Vector x of camera system, all vectors of camera system have to be ortonormal */
	Ogre::Vector3 cameraY; /** Vector y of camera system, all vectors of camera system have to be ortonormal */
	Ogre::Vector3 cameraZ; /** Vector z of camera system, all vectors of camera system have to be ortonormal */   

	int maxRayBounce; /** Maximum ray trace value */ 
};

} //namespace rendering
#endif // _SCENE_HPP_