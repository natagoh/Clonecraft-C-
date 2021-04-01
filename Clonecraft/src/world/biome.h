#ifndef BIOME_HPP
#define BIOME_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "render/mesh.h"
#include "render/texture.h"
#include "world/block.h"

class Biome {
private:
	enum class BiomeType {
		GRASSLAND,
		FOREST,
		DESERT,
	};

public:
	Biome();
};

#endif