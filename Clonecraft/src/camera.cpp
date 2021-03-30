#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() = default;

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
	this->position = position;
	this->direction = direction;
	this->up = up;
}

void Camera::setPosition(glm::vec3 position) {
	this->position = position;
}

glm::vec3 Camera::getPosition() {
	return this->position;
}

void Camera::setDirection(glm::vec3 direction) {
	this->direction = direction;
}

glm::vec3 Camera::getDirection() {
	return this->direction;
}

glm::vec3 Camera::getUp() {
	return this->up;
}

glm::mat4 Camera::getView() {
	glm::mat4 view = glm::lookAt(this->position, this->position + this->direction, this->up);
	return view;
}

void Camera::setSpeed(float speed) {
	this->speed = speed;
}

float Camera::getSpeed() {
	return this->speed;
}
