#include "frustum.h"
#include "render/mesh.h"
#include <glm/gtc/constants.hpp>
#include <glad/glad.h>
#include <vector>

#include <iostream>

Frustum::Frustum(Camera camera, glm::mat4 projection) {
	this->camera = camera;
	this->projection = projection;

	// extract info from projection matrix
	fov = 2.0f * atan(1.0f / projection[1][1]) * 180.0f / glm::pi<float>();
	aspect_ratio = projection[1][1] / projection[0][0];
	near = projection[3][2] / (projection[2][2] - 1.0f);
	far = projection[3][2] / (projection[2][2] + 1.0f);
	//std::cout << "fov " << fov << " aspect " << aspect_ratio << " near " << near << " far " << far << std::endl;
}

void Frustum::generateVertices() {
	// get camera vectors
	glm::vec3 pos = camera.getPosition();
	glm::vec3 dir = camera.getDirection();
	glm::vec3 up = camera.getUp();
	glm::vec3 right = glm::normalize(glm::cross(dir, up));

	// near and far coords
	glm::vec3 n_pos = pos + near * dir;
	glm::vec3 f_pos = pos + far * dir;

	// dimensions of near and far planes
	float n_height = 2 * tan(fov / 2) * near;
	float n_width = n_height * aspect_ratio;

	float f_height = 2 * tan(fov / 2) * far;
	float f_width = f_height * aspect_ratio;

	// ntl = near top left
	ntl = n_pos + up * (n_height / 2.0f) - right * (n_width / 2.0f);
	nbl = n_pos - up * (n_height / 2.0f) - right * (n_width / 2.0f);
	ntr = n_pos + up * (n_height / 2.0f) + right * (n_width / 2.0f);
	nbr = n_pos - up * (n_height / 2.0f) - right * (n_width / 2.0f);

	ftl = f_pos + up * (f_height / 2.0f) - right * (f_width / 2.0f);
	fbl = f_pos - up * (f_height / 2.0f) - right * (f_width / 2.0f);
	ftr = f_pos + up * (f_height / 2.0f) + right * (f_width / 2.0f);
	fbr = f_pos - up * (f_height / 2.0f) - right * (f_width / 2.0f);

	// construct frustum planes
	planes[FrustumPlane::BOTTOM] = Plane{ nbl, nbr, fbr };
	planes[FrustumPlane::TOP] = Plane{ ntl, ntr, ftr };
	planes[FrustumPlane::LEFT] = Plane{ nbl, ntl, fbl };
	planes[FrustumPlane::RIGHT] = Plane{ nbr, ntr, fbr };
	planes[FrustumPlane::NEAR] = Plane{ nbl, nbr, ntr };
	planes[FrustumPlane::FAR] = Plane{ fbl, fbr, ftr };
}

// render frustum for debugging
void Frustum::render() {
	std::vector<GLfloat> vertices;

	vertices.push_back(ntl.x);
	vertices.push_back(ntl.y);
	vertices.push_back(ntl.z);

	vertices.push_back(nbl.x);
	vertices.push_back(nbl.y);
	vertices.push_back(nbl.z);

	vertices.push_back(ntr.x);
	vertices.push_back(ntr.y);
	vertices.push_back(ntr.z);

	vertices.push_back(nbr.x);
	vertices.push_back(nbr.y);
	vertices.push_back(nbr.z);

	vertices.push_back(ftl.x);
	vertices.push_back(ftl.y);
	vertices.push_back(ftl.z);

	vertices.push_back(fbl.x);
	vertices.push_back(fbl.y);
	vertices.push_back(fbl.z);

	vertices.push_back(ftr.x);
	vertices.push_back(ftr.y);
	vertices.push_back(ftr.z);

	vertices.push_back(fbr.x);
	vertices.push_back(fbr.y);
	vertices.push_back(fbr.z);

	// ntl 0
	// nbl 1
	// ntr 2
	// nbr 3
	// ftl 4
	// fbl 5
	// ftr 6
	// fbr 7

	planes[FrustumPlane::BOTTOM] = Plane{ nbl, nbr, fbr };
	planes[FrustumPlane::TOP] = Plane{ ntl, ntr, ftr };
	planes[FrustumPlane::LEFT] = Plane{ nbl, ntl, fbl };
	planes[FrustumPlane::RIGHT] = Plane{ nbr, ntr, fbr };
	planes[FrustumPlane::NEAR] = Plane{ nbl, nbr, ntr };
	planes[FrustumPlane::FAR] = Plane{ fbl, fbr, ftr };

	std::vector<GLuint> indices = {
		1, 2, 0, 1, 3, 2,	// near
		5, 6, 4, 5, 7, 6,	//far
	};

	Mesh mesh(vertices, indices);
	mesh.render();
	mesh.cleanup();

}