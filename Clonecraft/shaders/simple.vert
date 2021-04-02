#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 frag_uv;
//out float ao;

uniform mat4 mvp;	// model view projection matrix

const vec3 light_direction = normalize(vec3(-1.0, 1.0, -1.0));

void main() {
	gl_Position = mvp * vec4(position, 1);
	frag_uv = uv;
	// ambient occlusion
	//ao = 0.3 + (1.0 - uv.z) * 0.7;
}