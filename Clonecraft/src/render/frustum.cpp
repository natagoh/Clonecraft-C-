#include "frustum.h"
#include "render/mesh.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>

#include <iostream>

Frustum::Frustum(Camera camera, glm::mat4 projection) {
	this->camera = camera;
	this->projection = projection;

	// extract info from projection matrix
	fov = 2.0f * atan(1.0f / projection[1][1]) * 180.0f / glm::pi<float>();
	aspect_ratio = projection[1][1] / projection[0][0];
	near = projection[3][2] / (projection[2][2] - 1.0f);
	far = projection[3][2] / (projection[2][2] + 1.0f);
	std::cout << "fov " << fov << " aspect " << aspect_ratio << " near " << near << " far " << far << std::endl;
}

// tests if point is inside frustum
// true = inside, false = outside
bool Frustum::pointIntersection(glm::vec3 point) {
	for (int i = FrustumPlane::TOP; i <= FrustumPlane::FAR; i++) {
		Plane plane = planes[i];
		float dist = glm::dot(plane.n, point - plane.p);
		//std::cout << "distance from point " << glm::to_string(point) << " to plane " << i << " is: " << dist << std::endl;
		if (dist < 0.0f) return false;
		//std::cout << "distance from point to plane: " << dist << std::endl;
	}
	return true;
}

// for cube to be inside frustum, at least 1 vertex should be inside
bool Frustum::cubeIntersection(std::vector<glm::vec3> vertices) {
	for (glm::vec3& vertex : vertices) {
		if (pointIntersection(vertex)) {
			return true;
		}
	}
	return false;
}

void Frustum::generatePlanes() {
	// get camera vectors
	/*glm::vec3 pos = camera.getPosition();
	glm::vec3 dir = camera.getDirection();
	glm::vec3 up = camera.getUp();
	glm::vec3 right = glm::normalize(glm::cross(dir, up));*/

	glm::vec3 pos = glm::vec3(20.0f, 20.0f, 20.0f);
	glm::vec3 dir = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(dir, up));

	// near and far coords
	glm::vec3 n_pos = pos + near * dir;
	glm::vec3 f_pos = pos + far * dir;

	// dimensions of near and far planes
	float n_height = 2 * tan(fov / 2) * near;
	float n_width = n_height * aspect_ratio;

	float f_height = 2 * tan(fov / 2) * far;
	float f_width = f_height * aspect_ratio;
	
	glm::vec3 n_h = up * (n_height / 2.0f);
	glm::vec3 n_w = right * (n_width / 2.0f);

	glm::vec3 f_h = up * (f_height / 2.0f);
	glm::vec3 f_w = right * (f_width / 2.0f);

	// frustum vertices
	ntl = n_pos + n_h - n_w;
	nbl = n_pos - n_h - n_w;
	ntr = n_pos + n_h + n_w;
	nbr = n_pos - n_h + n_w;

	ftl = f_pos + f_h - f_w;
	fbl = f_pos - f_h - f_w;
	ftr = f_pos + f_h + f_w;
	fbr = f_pos - f_h + f_w;
	
	// plane normals
	glm::vec3 near_normal = glm::normalize(glm::cross(glm::vec3(nbl - nbr), glm::vec3(ntr - nbr)));
	glm::vec3 far_normal = glm::normalize(glm::cross(glm::vec3(fbr - fbl), glm::vec3(ftl - fbl)));
	glm::vec3 left_normal = glm::normalize(glm::cross(glm::vec3(ftl - fbl), glm::vec3(nbl - fbl)));
	glm::vec3 right_normal = glm::normalize(glm::cross(glm::vec3(nbr - fbr), glm::vec3(ftr - fbr)));
	glm::vec3 top_normal = glm::normalize(glm::cross(glm::vec3(ntl - ntr), glm::vec3(ftr - ntr)));
	glm::vec3 bottom_normal = glm::normalize(glm::cross(glm::vec3(fbl - fbr), glm::vec3(nbr - fbr)));

	// construct frustum planes
	planes[FrustumPlane::NEAR] = Plane{ near_normal, nbr };
	planes[FrustumPlane::FAR] = Plane{ far_normal,  fbr };
	planes[FrustumPlane::LEFT] = Plane{ left_normal, fbl };
	planes[FrustumPlane::RIGHT] = Plane{ right_normal, fbr };
	planes[FrustumPlane::TOP] = Plane{ top_normal, ftr };
	planes[FrustumPlane::BOTTOM] = Plane{ bottom_normal, fbr };
}

// render frustum for debugging
// todo: currently very broken
void Frustum::render() {
	std::vector<GLfloat> vertices;

	for (int i = 0; i < 3; i++) {
		vertices.push_back(ntl[i]);
	}
	for (int i = 0; i < 3; i++) {
		vertices.push_back(nbl[i]);
	}
	for (int i = 0; i < 3; i++) {
		vertices.push_back(ntr[i]);
	}
	for (int i = 0; i < 3; i++) {
		vertices.push_back(nbr[i]);
	}

	for (int i = 0; i < 3; i++) {
		vertices.push_back(ftl[i]);
	}
	for (int i = 0; i < 3; i++) {
		vertices.push_back(fbl[i]);
	}
	for (int i = 0; i < 3; i++) {
		vertices.push_back(ftr[i]);
	}
	for (int i = 0; i < 3; i++) {
		vertices.push_back(fbr[i]);
	}

	// render points
	std::vector<GLuint> indices = {
		0, 1, 2, 3, 4, 5, 6, 7,
	};

	Mesh mesh(vertices, indices);
	mesh.render(GL_POINTS);
	mesh.cleanup();

	// render lines
	indices.clear();
	indices = {
		0, 1, 1, 3, 3, 2, 2, 0, // front
		4, 6, 6, 7, 7, 5, 5, 4, // back
		0, 4,
		2, 6,
		1, 5,
		3, 7,
	};

	mesh = Mesh(vertices, indices);
	mesh.render(GL_LINES);
	mesh.cleanup();
}