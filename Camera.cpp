

#include "Camera.h"
#include "glm\glm\glm.hpp"
#include "math.h"
#include <cmath>
#include <fstream>


using namespace std;


Camera::Camera() {

	reso_w = 0.0;
	reso_h = 0.0;

	setCamera();
}


Camera::Camera(glm::vec3 E_config, glm::vec3 C_config, glm::vec3 U_config,
	           float FOVY_config, int reso_w_config, int reso_h_config) {

	E = E_config;
	C = C_config;
	U = U_config;
	Camera::reso_w = reso_w_config;
	Camera::reso_h = reso_h_config;
	FOVY = FOVY_config;

	setCamera();
}


Camera::~Camera() {
}


void Camera::setCamera() {

	//convert FOVY from degrees to radians
	FOVY *= 3.14159 / 180.0;

	// FOV: field of view half-angles: FOVY given, FOVX = FOVY(reso_w/reso_h)
	FOVX = atan(tan(FOVY) * (reso_w-1)/(reso_h-1));
	
	A = glm::cross(C, U);	// A <-- C x U
	B = glm::cross(A, C);	// B <-- A x C

	M = E + C;				// M <-- E + C  (screen midpoint)

	// H <-- (A |C| tan FOVX) / |A|
	H = A * ((glm::length(C) * tan(FOVX)) / glm::length(A));

	// V <-- (B |C| tan FOVY) / |B|
	V = B * ((glm::length(C) * tan(FOVY)) / glm::length(B));


	// NDC coordinates
	float sx, sy = 0.0;

	// world point array
	Pw_arr.resize(reso_w * reso_h);
	int pw_index = 0;

	for (int py = 0; py < reso_h; py++) {
		for (int px = 0; px < reso_w; px++) {

			// Get NDC coordinates using resolution and current pixel value.
			sx = (float) px / reso_w;
			sy = (float) py / reso_h;

			// Convert the point into world space.
			glm::vec3 num = M + ((2 * sx - 1) * H) + ((1 - 2 * sy) * V);
			Pw_arr[pw_index] = num;
			++pw_index;
		}
	}
}

glm::vec3 Camera::GetWorldPointVec(int px, int py) {
	float sx, sy = 0.0;

	// Get NDC coordinates using resolution and current pixel value.
	sx = (float) px / reso_w;
	sy = (float) py / reso_h;

	// Convert the point into world space.
	glm::vec3 num = M + ((2 * sx - 1) * H) + ((1 - 2 * sy) * V);
	return num;

}









