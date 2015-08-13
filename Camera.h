

#include <cmath>
#include "math.h"
#include <string>
#include "glm\glm\glm.hpp"
#include <vector>

#ifndef CAMERA
#define CAMERA


using namespace std;

class Camera {

public:

	glm::vec3 E;	// E: eye/camera world coordinates
	glm::vec3 C;	// C: viewing vector direction
	glm::vec3 U;	// U: up vector

	float FOVX;		// horizontal FOV half-angle
	float FOVY;		// vertical FOV half-angle

	int reso_w;		// screen width
	int reso_h;		// screen height

	glm::vec3 A;	// A <-- C x U
	glm::vec3 B;	// B <-- A x C
	glm::vec3 M;	// M <-- E + C
	glm::vec3 H;	// H <-- (A |C| tan FOVX) / |A|
	glm::vec3 V;	// V <-- (B |C| tan FOVY) / |B|

	std::vector<glm::vec3> Pw_arr;		// array of world points as vectors

	Camera();
	Camera(glm::vec3 E_config,
		   glm::vec3 C_config,
		   glm::vec3 U_config,
		   float FOVY_config,
		   int reso_w_config,
		   int reso_h_config);
	~Camera();

	void setCamera();

	glm::vec3 GetWorldPointVec(int pix_x, int pix_y);

};



#endif