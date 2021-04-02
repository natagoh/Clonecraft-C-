#ifndef BIOME_HPP
#define BIOME_HPP

#include "world/block.h"
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
	static GLshort* generateHeightMap(glm::vec3 position);
	static BlockType getBlockType(int elevation, int height);
};

#endif