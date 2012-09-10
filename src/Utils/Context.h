#ifndef _CONTEXT_H_
#define _CONTEXT_H_
#include <vector>

using namespace std;

#include <boost/serialization/vector.hpp>
class Context {
public:
	class Light {
	private:
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{ 
			ar & position;
			ar & r;
			ar & ambient;
			ar & diffuse;
			ar & specular;	
		}
	public:
        string id;
		float position[3];
		float r; // jak duze swiatlo jest, potrzebne zeby dalo sie w nie trafic
		float ambient[3]; //kolory oswietrelniea wg modelu Phonga
		float diffuse[3];
		float specular[3]; 
	};
	class Sphere {
	private:
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{ 
			ar & position;
			ar & r;
			ar & ambient;
			ar & diffuse;
			ar & specular;	
		}
	public:
        string id;
		float position[3];
		float r;
		float ambient[3];
		float diffuse[3];
		float specular[3];
		int n; //wsp zwierciadlowosci
	};


private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & dimension;
		ar & window;
		ar & viewportSize;
		ar & background;
		ar & globalLight;
		ar & lights;
		ar & spheres;
		ar & cameraPosition;
		ar & cameraX;
		ar & cameraY;
		ar & cameraZ;
		ar & maxRayBounce;
	}
public:
  int dimension[2]; //px
  int window[2][2]; //rozmiar czesci, 2 punkty prostokata
  float viewportSize[3]; //rozmiar ukladu rzeczywistego

  float background[3]; //kolor tla
  float globalLight[3]; //oswietlenie globalne ambient

  std::vector < Light > lights;
  std::vector < Sphere > spheres;

  string cameraId;
  float cameraPosition[3]; //punkt kamery
  float cameraX[3]; // wektory ukladu kamery
  float cameraY[3];
  float cameraZ[3]; // wektory musza byc ortonormalne
  //kamera "patrzy" w przeciwna strone niz wektor Z

  int maxRayBounce; //glebia sledzenia promieni (ok 3 najlepiej)
};
#endif
