#version 430 core

in vec2 frag_uv;
in vec3 frag_normal;

out vec4 out_color;

uniform sampler2D texture_sampler;

const vec3 light_color = vec3(1.0, 1.0, 1.0);
const vec3 light_direction = normalize(vec3(-1.0, 1.0, -1.0));
const float ambient_strength = 0.1;
const float water_transparency = 0.75;

void main() {
	// ambient
    vec3 ambient = ambient_strength * light_color;

	// difuse
	float diff = clamp(dot(frag_normal, light_direction), 0.0, 1.0);
	vec3 diffuse = diff * light_color;

	// color from the texture
	vec4 object_color = texture(texture_sampler, frag_uv);
	vec3 result = (ambient + diffuse) * vec3(object_color);
	out_color = vec4(result, object_color[3] * water_transparency);
	
	// debugging normals
	//out_color.rgb = 0.5 + 0.5 * frag_normal; 
  }