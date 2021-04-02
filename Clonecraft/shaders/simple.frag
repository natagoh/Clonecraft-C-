#version 430 core

in vec2 frag_uv;

out vec4 color;

uniform sampler2D texture_sampler;

void main() {
	 //color = vec4(.5, 0, 0, 1);
	color = texture(texture_sampler, frag_uv);
}