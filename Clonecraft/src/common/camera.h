#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
	float speed;

public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();

	void setDirection(glm::vec3 direction);
	glm::vec3 getDirection();

	glm::vec3 getUp();

	glm::mat4 getView();

	void setSpeed(float speed);
	float getSpeed();
};

#endif
