
#define _CRT_SECURE_NO_WARNINGS

#include "FileReader.h"
#include "glm\glm\glm.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//Constructor:
FileReader::FileReader(char * file) {
		
	ifstream configFile(file);

	if (!configFile.is_open())
		cout << "Could not open config file." << endl;

	while (!configFile.eof()) {

		char line[100];
		char * param[4];

		configFile.getline(line, 100);
		param[0] = strtok(line, " ");

		if (!param[0]) continue;

		//step - float, step size
		if (strcmp(param[0], "STEP") == 0) {
			param[1] = strtok(0, " ");
			step = stof(param[1], NULL);

		//xyzc - vector of floats, voxel buffer dimensions
		} else if (strcmp(param[0], "XYZC") == 0) {
			param[1] = strtok(0, " ");
			param[2] = strtok(0, " ");
			param[3] = strtok(0, " ");
			voxb_w = stoi(param[1], NULL);
			voxb_h = stoi(param[2], NULL);
			voxb_d = stoi(param[3], NULL);
			xyzc = glm::vec3(voxb_w, voxb_h, voxb_d);

			voxbuffer = new VoxelBuffer(xyzc);

		//bg_rgb - vector of floats, background color
		} else if (strcmp(param[0], "BRGB") == 0) {
			param[1] = strtok(0, " ");
			param[2] = strtok(0, " ");
			param[3] = strtok(0, " ");
			float bg_r = stof(param[1], NULL);
			float bg_g = stof(param[2], NULL);
			float bg_b = stof(param[3], NULL);
			bg_rgb = glm::vec3(bg_r, bg_g, bg_b);

			voxbuffer->bg_rgb = bg_rgb;

		//mg_rgb - vector of floats, material color
		} else if (strcmp(param[0], "MRGB") == 0) {
			param[1] = strtok(0, " ");
			param[2] = strtok(0, " ");
			param[3] = strtok(0, " ");
			float ma_r = stof(param[1], NULL);
			float ma_g = stof(param[2], NULL);
			float ma_b = stof(param[3], NULL);
			ma_rgb = glm::vec3(ma_r, ma_g, ma_b);

			voxbuffer->ma_rgb = ma_rgb;
		
		//output_filename - string, name of output file
		} else if (strcmp(param[0], "FILE") == 0) {
			param[1] = strtok(0, " ");
			output_filename = new char[strlen(param[1])];
			strcpy(output_filename, param[1]);

		//reso_w & reso_h - ints, screen size/resolution
		} else if (strcmp(param[0], "RESO") == 0) {
			param[1] = strtok(0, " ");
			param[2] = strtok(0, " ");
			reso_w = stoi(param[1], NULL);
			reso_h = stoi(param[2], NULL);

		//E_config - vector of floats, eye/camera world coordinates
		} else if (strcmp(param[0], "EYEP") == 0) {
			param[1] = strtok(0, " ");
			param[2] = strtok(0, " ");
			param[3] = strtok(0, " ");
			float eyep_x = stof(param[1], NULL);
			float eyep_y = stof(param[2], NULL);
			float eyep_z = stof(param[3], NULL);
			E_config = glm::vec3(eyep_x, eyep_y, eyep_z);

		//C_config - vector of floats, viewing vector direction
		} else if (strcmp(param[0], "VDIR") == 0) {
			param[1] = strtok(0, " ");
			param[2] = strtok(0, " ");
			param[3] = strtok(0, " ");
			float vdir_x = stof(param[1], NULL);
			float vdir_y = stof(param[2], NULL);
			float vdir_z = stof(param[3], NULL);
			C_config = glm::vec3 (vdir_x, vdir_y, vdir_z);

		//U_config - vector of floats, up vector
		} else if (strcmp(param[0], "UVEC") == 0) {
			param[1] = strtok(0, " ");
			param[2] = strtok(0, " ");
			param[3] = strtok(0, " ");
			float uvec_x = stof(param[1], NULL);
			float uvec_y = stof(param[2], NULL);
			float uvec_z = stof(param[3], NULL); 
			U_config = glm::vec3 (uvec_x, uvec_y, uvec_z);

		//fovy - float, half-angle field of view in y-direction
		} else if (strcmp(param[0], "FOVY") == 0) {
			param[1] = strtok(0, " ");
			fovy = stof(param[1], NULL);

		//light_pos - vector of floats, light position
		} else if (strcmp(param[0], "LPOS") == 0) {
			param[1] = strtok(0, " ");
			param[2] = strtok(0, " ");
			param[3] = strtok(0, " ");
			float lpos_x = stof(param[1], NULL);
			float lpos_y = stof(param[2], NULL);
			float lpos_z = stof(param[3], NULL);
			light_pos = glm::vec3(lpos_x, lpos_y, lpos_z);

			voxbuffer->light_pos = light_pos;
	
		//light_col - vector of floats, light color
		} else if (strcmp(param[0], "LCOL") == 0) {
			param[1] = strtok(0, " ");
			param[2] = strtok(0, " ");
			param[3] = strtok(0, " ");
			float lcol_r = stof(param[1], NULL);
			float lcol_g = stof(param[2], NULL);
			float lcol_b = stof(param[3], NULL);				
			light_col = glm::vec3(lcol_r, lcol_g, lcol_b);

			voxbuffer->light_rgb = light_col;

		//voxbuffer.setSphereVoxels(center, radius)
		} else if (strcmp(param[0], "sphere") == 0) {
			numPrimitives++;
			primType = param[0];

			configFile.getline(line, 100);
			param[0] = strtok(line, " ");
			float xCenter = stof(param[0], NULL);
			float yCenter = stof(param[1], NULL);
			float zCenter = stof(param[2], NULL);
			center = glm::vec3(xCenter, yCenter, zCenter);

			configFile.getline(line, 100);
			param[0] = strtok(line, " ");
			radius = stof(param[0], NULL);

			voxbuffer->setSphereVoxels(center, radius);

		//voxbuffer.setPyroVoxels(center, radius)
		} else if (strcmp(param[0], "pyroclastic") == 0) {
			numPrimitives++;
			primType = param[0];
				
			configFile.getline(line, 100);
			param[0] = strtok(line, " ");
			float xCenter = stof(param[0], NULL);
			float yCenter = stof(param[1], NULL);
			float zCenter = stof(param[2], NULL);
			center = glm::vec3(xCenter, yCenter, zCenter);

			configFile.getline(line, 100);
			param[0] = strtok(line, " ");
			radius = stof(param[0], NULL);

			voxbuffer->setPyroVoxels(center, radius);

		//voxbuffer.setCloudVoxels(center, radius)
		} else if (strcmp(param[0], "cloud") == 0) {
			numPrimitives++;
			primType = param[0];

			configFile.getline(line, 100);
			param[0] = strtok(line, " ");
			float xCenter = stof(param[0], NULL);
			float yCenter = stof(param[1], NULL);
			float zCenter = stof(param[2], NULL);
			center = glm::vec3(xCenter, yCenter, zCenter);

			configFile.getline(line, 100);
			param[0] = strtok(line, " ");
			radius = stof(param[0], NULL);

			voxbuffer->setCloudVoxels(center, radius);
		}
	}
}


//step - float, step size for ray marching
float FileReader::get_step() {
	return step;
}

//xyzc - vector of floats, voxel buffer dimensions
glm::vec3 FileReader::get_XYZC() {
	return xyzc;
}

//bg_rgb - vector of floats, background color
glm::vec3 FileReader::get_BCOL() {
	return bg_rgb;
}

//mg_rgb - vector of floats, material color
glm::vec3 FileReader::get_MCOL() {
	return ma_rgb;
}

//output_filename - string, name of output file
char* FileReader::get_FILE() {
	return output_filename;
}

//reso_w - int, screen size/resolution width
int FileReader::get_W() {
	return reso_w;
}

//reso_h - int, screen size/resolution height
int FileReader::get_H() {
	return reso_h;
}

//E_config - vector of floats, eye/camera world coordinates
glm::vec3 FileReader::get_E() {
	return E_config;
}

//C_config - vector of floats, viewing vector direction
glm::vec3 FileReader::get_C() {
	return C_config;
}

//U_config - vector of floats, up vector
glm::vec3 FileReader::get_U() {
	return U_config;
}

//fovy - float, half-angle field of view in y-direction
float FileReader::get_FOVY() {
	return fovy;
}

//light_pos - vector of floats, light position
glm::vec3 FileReader::get_LPOS() {
	return light_pos;
}

//light_col - vector of floats, light color
glm::vec3 FileReader::get_LCOL() {
	return light_col;
}

//total number of primitives
int FileReader::get_numPrim() {
	return numPrimitives;
}
	
//initiate voxel buffer
VoxelBuffer* FileReader::get_VB() {
	return voxbuffer;
}