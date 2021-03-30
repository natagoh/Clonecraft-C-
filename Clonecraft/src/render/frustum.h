#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

#include <glm/glm.hpp>

#include "camera.h"

class Frustum {
private:
	// we can define a plane with 3 points
	struct Plane {
		glm::vec3 p0;
		glm::vec3 p1;
		glm::vec3 p2;
	};

	// to access frustum planes
	enum FrustumPlane {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEAR,
		FAR,
	};

	Camera camera;
	glm::mat4 projection;

	float fov, aspect_ratio, near, far;
	glm::vec3 ntl, nbl, ntr, nbr, ftl, fbl, ftr, fbr;
	Plane planes[6] = {};

public:
	Frustum(Camera camera, glm::mat4 projection);

	void generateVertices();
	bool pointTest(glm::vec3 point);

	void render();
};

#endif