#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "render/mesh.h"
#include "render/texture.h"
#include "world/block.h"

#define CHUNK_SIZE 16

class Chunk {
private:
	Block blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	//Mesh mesh;

public:
	Chunk();
	//~Chunk();

	Block getBlockAt(int x, int y, int z);
	bool isVisibleAt(int x, int y, int z);
	void generateMesh();
	//void addBlockToMesh(int x, int y, int z);
	
};

#endif