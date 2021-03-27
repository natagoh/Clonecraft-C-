#version 430 core

in vec2 outTexCoord;
out vec4 color;

uniform sampler2D texture_sampler;

void main() {
	// color = vec3(1, 0, 0);
	color = texture(texture_sampler, outTexCoord);
}