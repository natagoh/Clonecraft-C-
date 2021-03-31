#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

#include <glm/glm.hpp>
#include <vector>

#include "camera.h"

class Frustum {
private:
	// point-normal form
	// if a point x is on the plane, then n * (x - p) = 0;
	struct Plane {
		glm::vec3 n;
		glm::vec3 p;
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

	void generatePlanes();

	bool pointIntersection(glm::vec3 point);
	bool cubeIntersection(std::vector<glm::vec3> vertices);

	void render();
};

#endif