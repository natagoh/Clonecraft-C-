#ifndef BIOME_HPP
#define BIOME_HPP

#include <glm/glm.hpp>
#include <glad/glad.h>

class Biome {
private:
	enum class BiomeType {
		GRASSLAND,
		FOREST,
		DESERT,
	};

	Biome() {};

public:
	static GLubyte* generateHeightMap(glm::vec3 position);
};

#endif