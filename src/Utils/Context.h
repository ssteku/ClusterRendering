#ifndef _CONTEXT_H_
#define _CONTEXT_H_
#include <vector>
#include <string>
using namespace std;

/**
* Class represents one image which has to be rendered.
* It contains full description of the scene with all objects and parameters
*/
class Context {
public:
	/**
	* Class represents light object on the scene and contains all of it's parameters
	*/
	class Light {
	public:
		string id;
		/** Unique id of the light */
		float position[3]; /** Position of light [x][y][z] */
		float r;
		/** Ray of the light */
		float ambient[3];
		/** Ambient rgb colours in phong light description system */
		float diffuse[3]; /** Diffuse rgb colours in phong light description system*/
		float specular[3]; /** Specular rgb colours in phong light description system*/
	};

	/**
	* Class represents sphere object on the scene and contains all of it's parameters
	*/
	class Sphere {
	public:
		string id;
		/** Unique sphere id */
		float position[3]; /** Position of sphere [x][y][z] */
		float r; /** Ray size of the sphere */
		float ambient[3];
		/** Ambient rgb colours in phong light description system */
		float diffuse[3]; /** Diffuse rgb colours in phong light description system*/
		float specular[3];
		/** Specular rgb colours in phong light description system*/
		float n;
		/** Mirror factor of the sphere */
		float phongN; /* Phong factor */
		float blinnN; /* Blinn factor */
	};


	/**
	* Class represents sphere object on the scene and contains all of it's parameters
	*/
	class Triangle {
	public:
		string id;
		/** Unique sphere id */
		float point1[3],point2[3],point3[3]; /** Points defining triangle  */
		float normalVec[3]; /** Normal vector of triangle */
		float ambient[3];
		/** Ambient rgb colours in phong light description system */
		float diffuse[3]; /** Diffuse rgb colours in phong light description system*/
		float specular[3];
		/** Specular rgb colours in phong light description system*/
		float n;
		/** Mirror factor of the sphere */
		float phongN; /* Phong factor */
		float blinnN; /* Blinn factor */
	};


	/**
	* Class represents plane object on the scene and contains all of it's parameters
	*/
	class Plane {
	public:
		string id;
		/** Unique plane id */
		float normalVector[3]; /** Position of plane [x][y][z] normal vector */
		float distance; /** Distance from center (0,0,0) */
		float ambient[3];
		/** Ambient rgb colours in phong light description system */
		float diffuse[3]; /** Diffuse rgb colours in phong light description system*/
		float specular[3];
		/** Specular rgb colours in phong light description system*/
		float n;
		/** Mirror factor of the plane */
		float phongN; /* Phong factor */
		float blinnN; /* Blinn factor */
	};

	/**
	* Class represents box object on the scene and contains all of it's parameters
	*/
	class Box {
	public:
		string id;
		/** Unique box id */
		float minVector[3]; /** Minimum vector for box*/
		float maxVector[3]; /** Maximum vector for box*/
		float ambient[3];
		/** Ambient rgb colours in phong light description system */
		float diffuse[3]; /** Diffuse rgb colours in phong light description system*/
		float specular[3];
		/** Specular rgb colours in phong light description system*/
		float n;
		/** Mirror factor of the box */
		float phongN; /* Phong factor */
		float blinnN; /* Blinn factor */
	};

public:
	int antyAliasing;
	int dimension[2];
	/** Dimension of the result image(scene) */
	int window[2][2];
	/** Size of the part which has to be computed, describes two points of the rectangle*/
	float viewportSize[3];
	/** Size of the real system */

	float background[3];
	/** Background rgb colour */
	float globalLight[3];
	/** Ambient global rgb light colour */
	std::vector<Light> lights;
	/** Vector of lights on the scene */
	std::vector<Sphere> spheres;
	/** Vector of the spheres on the scene */
	std::vector<Plane> planes;
	/** Vector of the planes on the scene */
	std::vector<Box> boxes;
	/** Vector of the boxes on the scene */
	std::vector<Triangle> triangles;
	/** Vector of the triangles on the scene */
	string cameraId;
	/** Unique camera id */
	float cameraPosition[3]; /** Position of the camera [x][y][z] */
	/** Camera looks at the opposite side then vector specified below */
	float cameraX[3];
	/** Vector x of camera system, all vectors of camera system have to be ortonormal */
	float cameraY[3];
	/** Vector y of camera system, all vectors of camera system have to be ortonormal */
	float cameraZ[3];
	/** Vector z of camera system, all vectors of camera system have to be ortonormal */
	unsigned frameId;
	int maxRayBounce; /** Maximum ray trace value */
};
#endif
