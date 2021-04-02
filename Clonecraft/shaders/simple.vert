#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 texCoord;
//layout (location = 2) in vec3 normal;

out vec2 outTexCoord;

// model view projection matrix
uniform mat4 mvp;

void main() {
	gl_Position = mvp * vPosition;
	outTexCoord = texCoord;
}