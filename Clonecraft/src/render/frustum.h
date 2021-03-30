#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

#include <glm/glm.hpp>

#include "camera.h"


class Frustum {
private:
	Camera camera;
	glm::mat4 projection;

	float fov, aspect_ratio, near, far;

public:
	Frustum(Camera camera, glm::mat4 projection);
};

#endif