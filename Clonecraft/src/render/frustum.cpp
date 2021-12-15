#include "frustum.h"
#include "render/mesh.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>

#include <iostream>

Frustum::Frustum(Camera* camera, glm::mat4 projection) {
	this->camera = camera;
	this->projection = projection;

	// extract info from projection matrix
	fov = 2.0f * atan(1.0f / projection[1][1]); // in radians
	//fov = 0.436332; // testing purposes: 25 degrees
	aspect_ratio = projection[1][1] / projection[0][0];
	near = projection[3][2] / (projection[2][2] - 1.0f);
	far = projection[3][2] / (projection[2][2] + 1.0f);

	// dimensions of near and far planes
	n_height = 2.0f * tan(fov / 2.0f) * near;
	n_width = n_height * aspect_ratio;

	f_height = 2.0f * tan(fov / 2.0f) * far;
	f_width = f_height * aspect_ratio;

	std::cout << "fov " << fov << " aspect " << aspect_ratio << " near " << near << " far " << far << std::endl;
}

glm::mat4 Frustum::getView() {
	return this->camera->getView();
}

glm::mat4 Frustum::getProjection() {
	return projection;
}

// for cube to be inside frustum, at least 1 vertex should be inside
// true = inside, false = outside
bool Frustum::cubeIntersection(std::vector<glm::vec3> vertices) {
	for (int i = FrustumPlane::TOP; i <= FrustumPlane::FAR; i++) {
		int in = 0;
		for (glm::vec3& vertex : vertices) {
			Plane plane = planes[i];
			float dist = glm::dot(plane.n, vertex - plane.p);
			if (dist >= 0.0f) {
				in++;
			}

			// once we have a vertex inside, we know cube cannot be outside
			if (in != 0) {
				break;
			}
		}

		// no vertices inside means cube is outside frustum
		if (in == 0) {
			return false;
		}
	}
	return true;
}

void Frustum::generatePlanes() {
	glm::vec3 pos = camera->getPosition();
	glm::vec3 dir = camera->getDirection();
	glm::vec3 neg_dir = glm::vec3(0.0f) - dir;
	glm::vec3 right = glm::normalize(glm::cross(neg_dir, camera->getUp()));
	// actual camera up vector
	glm::vec3 up = glm::normalize(glm::cross(neg_dir, right));

	// near and far coords
	glm::vec3 n_pos = pos + near * dir;
	glm::vec3 f_pos = pos + far * dir;
	
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
	glm::vec3 near_normal = glm::normalize(glm::cross(nbl - nbr, ntr - nbr));
	glm::vec3 far_normal = glm::normalize(glm::cross(fbr - fbl, ftl - fbl));
	glm::vec3 left_normal = glm::normalize(glm::cross(ftl - fbl, nbl - fbl));
	glm::vec3 right_normal = glm::normalize(glm::cross(nbr - fbr, ftr - fbr));
	glm::vec3 top_normal = glm::normalize(glm::cross(ntl - ntr, ftr - ntr));
	glm::vec3 bottom_normal = glm::normalize(glm::cross(fbl - fbr, nbr - fbr));

	// construct frustum planes
	planes[FrustumPlane::NEAR] = Plane{ near_normal, nbr };
	planes[FrustumPlane::FAR] = Plane{ far_normal,  fbr };
	planes[FrustumPlane::LEFT] = Plane{ left_normal, fbl };
	planes[FrustumPlane::RIGHT] = Plane{ right_normal, fbr };
	planes[FrustumPlane::TOP] = Plane{ top_normal, ftr };
	planes[FrustumPlane::BOTTOM] = Plane{ bottom_normal, fbr };
}

// render frustum for debugging
void Frustum::render() {
	std::vector<GLfloat> vertices;
	glm::vec3 frustum_vertices[8] = {
		ntl, nbl, ntr, nbr,
		ftl, fbl, ftr, fbr,
	};

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			vertices.push_back(frustum_vertices[i][j]);
		}
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

	vertices.clear();
	indices.clear();
}