#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "render/mesh.h"

enum class BlockType {
	GRASS,
	AIR,
	DIRT,
	SAND,
};

class Block {
private:
	BlockType type = BlockType::GRASS;
	 
public:
	Block(void);
	Block(BlockType type);

	bool isVisible();
};

#endif
