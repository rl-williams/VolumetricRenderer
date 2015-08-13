

#include "Ray.h"
#include "glm\glm\glm.hpp"
#include <iostream>

using namespace std;

Ray::Ray() {

}


Ray::Ray(glm::vec3 Ro_config, glm::vec3 Rd_config) {

	Ro = Ro_config;
	Rd = Rd_config;

	BoxMin.x = 0.f;
	BoxMin.y = 0.f;
	BoxMin.z = 0.f;
	BoxMax.x = 1.f;
	BoxMax.y = 1.f;
	BoxMax.z = -1.f;

}


Ray::~Ray() {
}


bool Ray::intersects() {

	tnear = -FLT_MAX;
	tfar = FLT_MAX;

	//if ray is parallel to planes and origin is not between slabs, return false:
	if ((nearlyEquals(Rd.x, 0.f) && ((Ro.x < BoxMin.x) || (Ro.x > BoxMax.x))) ||
		(nearlyEquals(Rd.y, 0.f) && ((Ro.y < BoxMin.y) || (Ro.y > BoxMax.y))) ||
		(nearlyEquals(Rd.z, 0.f) && ((Ro.z < BoxMin.z) || (Ro.z > BoxMax.z)))) return false;


	/* X PLANE */

	//compute the intersection distance of the planes:
	t1 = (BoxMin.x - Ro.x) / Rd.x;
	t2 = (BoxMax.x - Ro.x) / Rd.x;

	//if (t1 > t2) swap (t1, t2), since t1 intersection with near plane
	if (t1 > t2) {
		float temp = t1;
		t1 = t2;
		t2 = temp;
	}

	//want largest tnear and smallest tfar
	if (t1 > tnear) tnear = t1;
	if (t2 < tfar) tfar = t2;



	/* Y PLANE */

	//compute the intersection distance of the planes:
	t1 = (BoxMin.y - Ro.y) / Rd.y;
	t2 = (BoxMax.y - Ro.y) / Rd.y;

	//if (t1 > t2) swap (t1, t2), since t1 intersection with near plane
	if (t1 > t2) {
		float temp = t1;
		t1 = t2;
		t2 = temp;
	}

	//want largest tnear and smallest tfar
	if (t1 > tnear) tnear = t1;
	if (t2 < tfar) tfar = t2;



	/* Z PLANE */

	//compute the intersection distance of the planes:
	t1 = (BoxMin.z - Ro.z) / Rd.z;
	t2 = (BoxMax.z - Ro.z) / Rd.z;

	//if (t1 > t2) swap (t1, t2), since t1 intersection with near plane
	if (t1 > t2) {
		float temp = t1;
		t1 = t2;
		t2 = temp;
	}

	//want largest tnear and smallest tfar
	if (t1 > tnear) tnear = t1;
	if (t2 < tfar) tfar = t2;



	//if box is missed, return false
	if ((tnear > tfar) || (tfar < 0)) return false;

	//if box survived all above tests, return true with intersection point tnear and exit point tfar
	intersectionPoint = tnear;
	exitPoint = tfar;
	return true;

}



bool Ray::nearlyEquals(float a, float b) {

	const float epsilon = 0.00001;

	const float absA = fabs(a);
    const float absB = fabs(b);
    const float diff = fabs(a - b);
	
	if (a == b) { // shortcut
		return true;
	} else if (a * b == 0) { // a or b or both are zero
		// relative error is not meaningful here
		return diff < (epsilon * epsilon);
	} else { // use relative error
		return diff / (absA + absB) < epsilon;
	}

}