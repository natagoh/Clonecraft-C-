#include "input.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "common/camera.h"

Input::Input(GLFWwindow* window, Camera* camera) {
    this->window = window;
    this->camera = camera;
}

void Input::handleKeyboard() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 pos = camera->getPosition();
    glm::vec3 dir = camera->getDirection();
    glm::vec3 up = camera->getUp();

    float cameraSpeed = camera->getSpeed();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->setPosition(pos + cameraSpeed * dir);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->setPosition(pos - cameraSpeed * dir);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->setPosition(pos - glm::normalize(glm::cross(dir, up)) * cameraSpeed);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->setPosition(pos + glm::normalize(glm::cross(dir, up)) * cameraSpeed);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera->setPosition(pos + cameraSpeed * up);
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera->setPosition(pos - cameraSpeed * up);
    }
}

void Input::handleMouseCursor() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float dx = xpos - lastX;
    float dy = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    dx *= this->sensitivity;
    dy *= this->sensitivity;

    yaw += dx;
    pitch += dy;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    camera->setDirection(glm::normalize(direction));
}

