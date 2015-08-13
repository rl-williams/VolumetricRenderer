

#include <cmath>
#include "math.h"
#include "glm\glm\glm.hpp"
#include <vector>

#ifndef RAY
#define RAY


using namespace std;

class Ray {

public:

	glm::vec3 Ro; //ray origin
	glm::vec3 Rd; //ray direction

	glm::vec3 BoxMin;  //(0, 0, 0)
	glm::vec3 BoxMax;  //(1, 1, -1)

	float tnear;
	float tfar;
	float t1;
	float t2;

	float intersectionPoint;
	float exitPoint;


	Ray();
	Ray(glm::vec3 Ro_config, glm::vec3 Rd_config);
	~Ray();

	bool intersects();

	bool nearlyEquals(float a, float b);

};



#endif