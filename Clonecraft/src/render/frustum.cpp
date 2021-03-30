#include "frustum.h"
#include <glm/gtc/constants.hpp>
#include <iostream>

Frustum::Frustum(Camera camera, glm::mat4 projection) {
	this->camera = camera;
	this->projection = projection;

	// extract info from projection matrix
	fov = 2.0 * atan(1.0 / projection[1][1]) * 180.0 / glm::pi<float>();
	aspect_ratio = projection[1][1] / projection[0][0];
	near = projection[3][2] / (projection[2][2]- 1.0);
	far = projection[3][2] / (projection[2][2] + 1.0);
	//std::cout << "fov " << fov << " aspect " << aspect_ratio << " near " << near << " far " << far << std::endl;
}


// right  glm::normalize(glm::cross(dir, up))