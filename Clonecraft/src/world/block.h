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
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	 
public:
	Block(void);
	Block(BlockType type);
	Block(BlockType type, glm::vec3 position);

	Mesh generateMesh();
	void render();
	glm::vec3 getPosition();
	bool isVisible();
};

#endif
