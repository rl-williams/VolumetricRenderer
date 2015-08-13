

#ifndef FILEREADER
#define FILEREADER

#include "glm\glm\glm.hpp"
#include "VoxelBuffer.h"

using namespace std;

class FileReader {

private:

	//variables read in from the config file:
	float step;
	int voxb_w, voxb_h, voxb_d;
	glm::vec3 xyzc;
	glm::vec3 bg_rgb;
	glm::vec3 ma_rgb;
	char * output_filename;
	int reso_w, reso_h;
	glm::vec3 E_config;
	glm::vec3 C_config;
	glm::vec3 U_config;
	float fovy;
	glm::vec3 light_pos;
	glm::vec3 light_col;
	int numPrimitives;
	string primType;
	glm::vec3 center;
	float radius;
	VoxelBuffer * voxbuffer;

public:

	//constructor:
	FileReader(char * file);

	//getters:
	float get_step();
	glm::vec3 get_XYZC();
	glm::vec3 get_BCOL();
	glm::vec3 get_MCOL();
	char* get_FILE();
	int get_W();
	int get_H();
	glm::vec3 get_E();
	glm::vec3 get_C();
	glm::vec3 get_U();
	float get_FOVY();
	glm::vec3 get_LPOS();
	glm::vec3 get_LCOL();
	int get_numPrim();
	VoxelBuffer * get_VB();
};

#endif