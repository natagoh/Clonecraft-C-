#ifndef INPUT_HPP
#define INPUT_HPP

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "camera.h"

class Input {
private:
	GLFWwindow* window;
	Camera* camera;
	bool firstMouse = true;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float sensitivity = 0.1f;

public:
	Input(GLFWwindow* window, Camera* camera);

	void handleKeyboard();
	void handleMouseCursor();
};

#endif
