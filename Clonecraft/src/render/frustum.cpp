#include "frustum.h"
#include "render/mesh.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>
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
	std::cout << "fov " << fov << " aspect " << aspect_ratio << " near " << near << " far " << far << std::endl;
}

// tests if point is inside frustum
// true = inside, false = outside
bool Frustum::pointTest(glm::vec3 point) {
	for (int i = FrustumPlane::TOP; i != FrustumPlane::FAR; i++) {
	//for (int i = FrustumPlane::TOP; i != FrustumPlane::BOTTOM; i++) {
		/*if (planes[i].GetPointDistance(point) < 0) {
			return false;
		}*/
		Plane plane = planes[i];
		float dist = glm::dot(plane.n, point - plane.p);
		//std::cout << "distance from point " << glm::to_string(point) << " to plane " << i << " is: " << dist << std::endl;
		if (dist > 0.0f) return true;
		//std::cout << "distance from point to plane: " << dist << std::endl;
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

	std::cout << "near position " << glm::to_string(n_pos) << std::endl;

	// dimensions of near and far planes
	float n_height = 2 * tan(fov / 2) * near;
	float n_width = n_height * aspect_ratio;

	float f_height = 2 * tan(fov / 2) * far;
	float f_width = f_height * aspect_ratio;
	
	glm::vec3 n_h = up * (n_height / 2.0f);
	glm::vec3 n_w = right * (n_width / 2.0f);

	glm::vec3 f_h = up * (f_height / 2.0f);
	glm::vec3 f_w = right * (f_width / 2.0f);

	std::cout << "near height " << glm::to_string(n_h) << std::endl;

	// ntl = near top left
	//ntl = n_pos + up * (n_height / 2.0f) - right * (n_width / 2.0f);
	//nbl = n_pos - up * (n_height / 2.0f) - right * (n_width / 2.0f);
	//ntr = n_pos + up * (n_height / 2.0f) + right * (n_width / 2.0f);
	//nbr = n_pos - up * (n_height / 2.0f) + right * (n_width / 2.0f);

	//ftl = f_pos + up * (f_height / 2.0f) - right * (f_width / 2.0f);
	//fbl = f_pos - up * (f_height / 2.0f) - right * (f_width / 2.0f);
	//ftr = f_pos + up * (f_height / 2.0f) + right * (f_width / 2.0f);
	//fbr = f_pos - up * (f_height / 2.0f) + right * (f_width / 2.0f);

	ntl = n_pos + n_h - n_w;
	nbl = n_pos - n_h - n_w;
	ntr = n_pos + n_h + n_w;
	nbr = n_pos - n_h + n_w;

	ftl = f_pos + f_h - f_w;
	fbl = f_pos - f_h - f_w;
	ftr = f_pos + f_h + f_w;
	fbr = f_pos - f_h + f_w;

	//// bring points into world space
	//glm::mat4 view = glm::lookAt(pos, pos + dir, up);
	//glm::mat4 view_inv = glm::inverse(view);

	////glm::mat4 mvp = projection * view;

	//ntl = view_inv * glm::vec4(ntl, 1);
	//nbl = view_inv * glm::vec4(nbl, 1);
	//ntr = view_inv * glm::vec4(ntr, 1);
	//nbr = view_inv * glm::vec4(nbr, 1);


	//ftl = view_inv * glm::vec4(ftl, 1);
	//fbl = view_inv * glm::vec4(fbl, 1);
	//ftr = view_inv * glm::vec4(ftr, 1);
	//fbr = view_inv * glm::vec4(fbr, 1);



	/*std::cout << "ntl " << glm::to_string(ntl) << std::endl;
	std::cout << "nbl " << glm::to_string(nbl) << std::endl;
	std::cout << "ntr " << glm::to_string(ntr) << std::endl;
	std::cout << "nbr " << glm::to_string(nbr) << std::endl;

	std::cout << "ftl " << glm::to_string(ftl) << std::endl;
	std::cout << "fbl " << glm::to_string(fbl) << std::endl;
	std::cout << "ftr " << glm::to_string(ftr) << std::endl;
	std::cout << "fbr " << glm::to_string(fbr) << std::endl;*/
	
	// plane normals
	glm::vec3 near_normal = glm::normalize(glm::cross(glm::vec3(nbl - nbr), glm::vec3(ntr - nbr)));
	glm::vec3 far_normal = glm::normalize(glm::cross(glm::vec3(fbr - fbl), glm::vec3(ftl - fbl)));
	glm::vec3 left_normal = glm::normalize(glm::cross(glm::vec3(ftl - fbl), glm::vec3(nbl - fbl)));
	glm::vec3 right_normal = glm::normalize(glm::cross(glm::vec3(nbr - fbr), glm::vec3(ftr - fbr)));
	glm::vec3 top_normal = glm::normalize(glm::cross(glm::vec3(ntl - ntr), glm::vec3(ftr - ntr)));
	glm::vec3 bottom_normal = glm::normalize(glm::cross(glm::vec3(fbl - fbr), glm::vec3(nbr - fbr)));

	//std::cout << "bottom_normal " << glm::to_string(bottom_normal) << std::endl;

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