
#include "VoxelBuffer.h"
#include "glm\glm\glm.hpp"
#include "perlin.h"
#include <iostream>
#include "Ray.h"
#include <cmath>
#include <algorithm>

using namespace std;

//Constructor:
VoxelBuffer::VoxelBuffer() {
	voxDensity_arr = new float[1]();
	voxLight_arr = new float[1]();
}


//Constructs a voxel buffer from the XYZC parameters from the config file
VoxelBuffer::VoxelBuffer(glm::vec3 xyzc_config) {

	//ADJUST PERLIN NOISE HERE:
	noise = new Perlin(4, 0.03f, 3.0f, 1); 

	v_width = xyzc_config.x;
	v_height = xyzc_config.y;
	v_depth = xyzc_config.z;

	int totalVox = v_width * v_height * v_depth;

	voxDensity_arr = new float[totalVox]();
	voxLight_arr = new float[totalVox]();

	//initialize all light values to -1 as a marker value:
	for (int i = 0; i < totalVox; i++) {
		voxLight_arr[i] = -1.0;
	}

	//initialize all density values to 0:
	for (int i = 0; i < totalVox; i++) {
		voxDensity_arr[i] = 0.0;
	}

}


//Deconstructor:
VoxelBuffer::~VoxelBuffer() {
	delete[] voxDensity_arr;
	delete[] voxLight_arr;
}


//Given any 3D coordinates, returns the voxel number that contains the point in space
int VoxelBuffer::getVoxNum(int p_x, int p_y, int p_z) {

	int voxNum = p_x + (v_width * p_y) + (v_width * v_height * p_z);

	if ((p_x >= v_width) || (p_y >= v_height) || (p_z >= v_depth) ||
		(voxNum > v_width*v_height*v_depth) || (voxNum < 0)) 
		return -1;

	return voxNum;
}



//Given the voxNum and a density value, set the element of the vox array to that density
void VoxelBuffer::setVoxDensity(int voxNum, float d) {
	voxDensity_arr[voxNum] = d;
}



//Given the voxel position and light position - secondary ray march from voxel to light source.
void VoxelBuffer::setVoxLight(glm::vec3 point_pos, glm::vec3 light_pos, float step, int voxNum) {

	glm::vec3 N = light_pos - point_pos;
	glm::vec3 D = glm::normalize(N);
	glm::vec3 ray = point_pos + D * step;

	float Q = 1.0;
	float dQ = 0.0;
	float k = 0.8;

	float length = glm::length(N);
	int maxSteps = length / step;
	int num_steps = 0;

	ray += D * step;

	while (Q > 0.0 && num_steps < maxSteps-1) {
		 
		int sx = ray.x * 100;
		int sy = ray.y * 100;
		int sz = ray.z * 100;
		sz = abs(sz);

		int vn = getVoxNum(sx, sy, sz);
		if (vn >= 0) {
			float p = voxDensity_arr[vn];
			if (p > 0) {
				dQ = exp(-k * p);
				Q *= dQ;
			}
		}

		ray += D * step;
		num_steps++;
	}

	Q = glm::clamp(Q, 0.0f, 1.0f);
	voxLight_arr[voxNum] = Q;
}



//Ray Marching
glm::vec3 VoxelBuffer::rayMarch(Ray eye, float step, bool triOn) {

	final_rgb = glm::vec3(0.0);

	float t = 1.0;  // t = total transmittance: 1 if clear, 0 if opaque
	float dt = 0.0;
	float k = 0.8;  // kappa
	float Q = 1.0;

	//R = O + tD
	glm::vec3 enter = eye.Ro + eye.intersectionPoint * eye.Rd;
	glm::vec3 exit = eye.Ro + eye.exitPoint * eye.Rd;

	//ray length and max steps
	float length = glm::length(exit - enter);
	int maxSteps = length / step;

	// for every step along the ray:
	glm::vec3 ray = enter;
	int num_steps = 0;
	
	//can stop when volume is essentially opaque as far as the ray has penetrated,
	// or when we've reached the max number of steps
	while (t >= 0.0 && num_steps < maxSteps) {

		float p = 0.0;

		//point in space:
		int sx = ray.x * 100;
		int sy = ray.y * 100;
		int sz = ray.z * 100;
		sz = abs(sz);

		//voxel number for that point
		int voxNum = getVoxNum(sx, sy, sz);

		//if the voxel exists, if we're within the voxel space:
		if (voxNum >= 0 && (voxNum < (v_width*v_height*v_depth))) {

			if  (triOn) p = triInterp(ray.x, ray.y, ray.z);
			if (!triOn) p = voxDensity_arr[voxNum];
		
			//if the current voxel has density > 0, Beer's Law:
			if (p > 0) {

				dt = exp(-k * p);
				t *= dt;
			
				// if light value == -1, need to compute the proper value and store it:
				if (voxLight_arr[voxNum] == -1.0f) {
					glm::vec3 point_pos = glm::vec3(ray.x, ray.y, ray.z);
					setVoxLight(point_pos, light_pos, step, voxNum);
				}

				Q *= voxLight_arr[voxNum];

				t = glm::clamp(t, 0.0f, 1.0f);
				Q = glm::clamp(Q, 0.0f, 1.0f);

				final_rgb += ((1 - dt) / k) * (ma_rgb * light_rgb) * t * Q;
			}
		}

		//increment to next step along the ray
		ray += step * eye.Rd;
		num_steps++;
	}

	final_rgb += t * bg_rgb;
	final_rgb = glm::clamp(final_rgb, 0.0f, 1.0f);
	return final_rgb;
}



//Tri-Linear Interpolation
float VoxelBuffer::triInterp(float x, float y, float z) {

	int xi = glm::floor(x * 100);
	int yj = glm::floor(y * 100);
	int zk = glm::floor(z * 100);
	zk = abs(zk);
	
	//make sure it's in the voxel space
	int vn = getVoxNum(xi, yj, zk);
	if (vn < 0) return 0.0;

	//densities
	float p = 0.0;
	float d = 0.0;
	int n = 0;

	for (int k = 0; k < 2; k++) {
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 2; i++) {

				//make sure it's in the voxel space
				vn = getVoxNum(xi + (i * 100), yj + (j * 100), zk + (k * 100));

				if (vn >= 0) {

					//weights
					float wx = 1.0 - ((x * 100) - (float) (xi + (i * 100)));
					float wy = 1.0 - ((y * 100) - (float) (yj + (j * 100)));
					float wz = 1.0 - ((z * 100) - (float) (zk + (k * 100)));
					float w = wx * wy * wz;
					w = glm::clamp(w, 0.0f, 1.0f);

					//p(x) += p(abc) * w(abc)
					d = voxDensity_arr[vn];
					p += d * w;
					n++;
				}
			}
		}
	}

	p = p / (float) n;

	return p;
}


void VoxelBuffer::setSphereVoxels(glm::vec3 center, float radius) {

	float d; //density
	int vox_num; //current voxel number
	glm::vec3 vox_pos; //current voxel position
	float distance; //distance from current voxel to center

	if (center.z > 1.0) center.z *= 0.01;

	center.x = glm::floor(center.x * 100);
	center.y = glm::floor(center.y * 100);
	center.z = glm::floor(center.z * 100);
	center.z = abs(center.z);

	radius *= 100;

	for (int z = 0; z < v_depth; z++) {
		for (int y = 0; y < v_height; y++) {
			for (int x = 0; x < v_width; x++) {

				vox_num = getVoxNum(x, y, z);
				vox_pos = glm::vec3(x, y, z);
				distance = glm::length(vox_pos - center);

				if (distance < radius) {
					d = voxDensity_arr[vox_num];
					d += (radius - distance)/radius;
					setVoxDensity(vox_num, d);
				}

			}
		}
	}
}


void VoxelBuffer::setPyroVoxels(glm::vec3 center, float radius) {
	
	float d; //density
	int vox_num; //current voxel number
	glm::vec3 vox_pos; //current voxel position
	float distance; //distance from current voxel to center

	if (center.z > 1.0) center.z *= 0.01;

	center.x = glm::floor(center.x * 100);
	center.y = glm::floor(center.y * 100);
	center.z = glm::floor(center.z * 100);
	center.z = abs(center.z);

	radius *= 100;

	for (int z = 0; z < v_depth; z++) {
		for (int y = 0; y < v_height; y++) {
			for (int x = 0; x < v_width; x++) {

				vox_num = getVoxNum(x, y, z);
				vox_pos = glm::vec3(x, y, z);

				glm::vec3 P = vox_pos - center;
				distance = glm::length(P);

				float n = noise->Get(P.x, P.y, P.z);

				//d = max((radius * Scale ­ |P / radius| + abs(fmb(P))), 0) * Scale
				d = max(radius - abs(distance/radius) + n, 0.0f);
				d += voxDensity_arr[vox_num];
				d = glm::clamp(d, 0.0f, 1.0f);

				setVoxDensity(vox_num, d);
			}
		}
	}
}


void VoxelBuffer::setCloudVoxels(glm::vec3 center, float radius) {
	
	float d; //density
	int vox_num; //current voxel number
	glm::vec3 vox_pos; //current voxel position
	float distance; //distance from current voxel to center

	if (center.z > 1.0) center.z *= 0.01;

	center.x = glm::floor(center.x * 100);
	center.y = glm::floor(center.y * 100);
	center.z = glm::floor(center.z * 100);
	center.z = abs(center.z);

	radius *= 100;

	for (int z = 0; z < v_depth; z++) {
		for (int y = 0; y < v_height; y++) {
			for (int x = 0; x < v_width; x++) {

				vox_num = getVoxNum(x, y, z);
				vox_pos = glm::vec3(x, y, z);

				glm::vec3 P = vox_pos - center;
				distance = glm::length(P);

				float n = noise->Get(P.x, P.y, P.z);

				//d = (fbm(P * Scale) + (1 ­ |P / radius|)) * Scale
				d = (n + (1 - abs(distance/radius)));
				d += voxDensity_arr[vox_num];
				d = glm::clamp(d, 0.0f, 1.0f);

				if (!(voxDensity_arr[vox_num] > 0.0f))
					setVoxDensity(vox_num, d);
			}
		}
	}
}


