
#include "glm\glm\glm.hpp"
#include "Ray.h"
#include "perlin.h"

#ifndef VOXELBUFFER
#define VOXELBUFFER

using namespace std;

class VoxelBuffer {

public:


	Perlin * noise;

	float * voxDensity_arr;
	float * voxLight_arr;

	glm::vec3 final_rgb;
	glm::vec3 ma_rgb;
	glm::vec3 bg_rgb;

	glm::vec3 light_rgb;
	glm::vec3 light_pos;

	int v_width;
	int v_height;
	int v_depth;

	//Constructors / deconstructor:
	VoxelBuffer();
	VoxelBuffer(glm::vec3 xyzc_config);
	~VoxelBuffer();

	//Given any 3D coordinates, returns the voxel number that contains the point in space:
	int getVoxNum(int p_x, int p_y, int p_z);

	//Given the voxNum and a density value, set the element of the vox array to that density
	void setVoxDensity(int voxNum, float d);

	//Given the voxel position and light position - secondary ray march from voxel to light source.
	void setVoxLight(glm::vec3 point_pos, glm::vec3 light_pos, float step, int voxNum);

	//Ray Marching
	glm::vec3 rayMarch(Ray eye, float step, bool triOn);

	//Tri-Linear Interpolation function
	float triInterp(float x, float y, float z);

	void setSphereVoxels(glm::vec3 center, float radius);
	void setPyroVoxels(glm::vec3 center, float radius);
	void setCloudVoxels(glm::vec3 center, float radius);
};

#endif