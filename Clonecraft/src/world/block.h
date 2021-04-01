#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "render/mesh.h"

enum BlockType: GLubyte {
	GRASS,
	AIR,
	DIRT,
	SAND,
};

class Block {
private:
	BlockType type = BlockType::AIR;
	 
public:
	Block(void);
	Block(BlockType type);

	BlockType getType();
	bool isVisible();
};

#endif
