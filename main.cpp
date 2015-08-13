/**
 *
 * Becky Williams
 * CIS 460 - HW1c
 * 
 *
 **/

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "EasyBMP.h"
#include "glm\glm\glm.hpp"
#include "Camera.h"
#include "Ray.h"
#include "VoxelBuffer.h"
#include "perlin.h"
#include "FileReader.h"


using namespace std;


int main(int argc, char** argv) {

	//TRILINEAR INTERPOLATION ON/OFF:
	bool triOn = true;


	/*** Read Config File ***/

	char * filename = argv[1];
	FileReader config = FileReader(filename);


	/*** Send Variables to Camera ***/

	glm::vec3 E_config = config.get_E();
	glm::vec3 C_config = config.get_C();
	glm::vec3 U_config = config.get_U();
	float fovy = config.get_FOVY();
	int reso_w = config.get_W();
	int reso_h = config.get_H();

	Camera cam = Camera(E_config, C_config, U_config, fovy, reso_w, reso_h);
	

	/*** Set Up Voxel Buffer ***/
	
	VoxelBuffer * voxBuffer = config.get_VB();


	/*** Create Output Image ***/

		BMP output;
		output.SetSize(reso_w, reso_h);
		output.SetBitDepth(24);

		glm::vec3 D;	// ray direction vector
		glm::vec3 Pw;   // eye world point vector
		glm::vec3 color = glm::vec3(0.0);
		glm::vec3 bg_rgb = config.get_BCOL();

		//for each pixel, send a ray from the eye through the pixel and into the scene:
		for(int y = 0; y < reso_h; y++) {
			cout << "Current Scanline: " << y << endl;

			for (int x = 0; x < reso_w; x++) {

				Pw = cam.GetWorldPointVec(x, y);
				D = glm::normalize(Pw - E_config);	// ray vector from world coordinates to pixels (x, y); normalized directional vector

				Ray ray = Ray(Pw, D);

				//if the ray intersects the voxel space:
				if (ray.intersects()) {

					//Ray Marching!
					float step = config.get_step();
					color = voxBuffer->rayMarch(ray, step, triOn);

					color *= 255.0f;
					color = glm::clamp(color, 0.0f, 255.0f);

					output(x, y)->Red   = color.r;
					output(x, y)->Green = color.g;
					output(x, y)->Blue  = color.b;

				//no ray marching for rays that don't intersect the buffer:
				} else {
					output(x, y)->Red   = bg_rgb.r * 255.0f;
					output(x, y)->Green = bg_rgb.g * 255.0f;
					output(x, y)->Blue  = bg_rgb.b * 255.0f;
				}
			}
		}
	

		char * output_file = config.get_FILE();
		output.WriteToFile(output_file);
		cout << "output has been written." << endl;

	return 0;
}