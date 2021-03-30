#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "render/mesh.h"
#include "render/texture.h"
#include "world/block.h"

#define CHUNK_DIM 16
#define NUM_BLOCKS CHUNK_DIM * CHUNK_DIM * CHUNK_DIM

class Chunk {
private:
	Block blocks[NUM_BLOCKS];
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::vector<GLfloat> uvs;

	Mesh mesh;

public:
	Chunk(void);
	//~Chunk();

	Block getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, Block block);
	bool blockIsVisibleAt(int x, int y, int z);
	void render();
	void generateMesh();
	void addBlockToMesh(int x, int y, int z);
	
};

#endif