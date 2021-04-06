#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 frag_uv;
out vec3 frag_normal;
//out float ao;

uniform mat4 mvp;	// model view projection matrix

void main() {
	gl_Position = mvp * vec4(position, 1);
	frag_uv = uv;
	frag_normal = normalize(mat3(mvp) * normal);
	// ambient occlusion
	//ao = 0.3 + (1.0 - uv.z) * 0.7;
}