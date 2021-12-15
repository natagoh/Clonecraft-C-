#version 430 core

layout (location = 0) in vec4 vPosition;

// model view projection matrix
uniform mat4 mvp;

void main() {
	gl_Position = mvp * vPosition;
}