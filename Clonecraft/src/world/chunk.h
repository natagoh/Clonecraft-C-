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
	enum BlockFace {
		FRONT = 0,
		BACK,
		RIGHT,
		LEFT,
		TOP,
		BOTTOM,
	};

	// using char since its only 16 bits
	static const unsigned char NUM_POINTS_PER_BLOCK = 24; // 6*4 points per block
	static const unsigned char NUM_POINTS_PER_FACE = 4;   // 4 points per face

	static const unsigned char FACE_VERTICES_OFFSET = 12; // because there are 4*3 vertex entries for each face
	static const unsigned char FACE_INDICES_OFFSET = 6;   // because there are 3*2 index entries for each face
	static const unsigned char FACE_UV_OFFSET = 8;        // because there are 2*4 uv entries for each face

	static const GLfloat base_vertices[];
	static const GLushort base_indices[];

	Block blocks[NUM_BLOCKS];
	glm::vec3 position = {};

	std::vector<GLfloat> vertices = {};
	std::vector<GLuint> indices = {};
	std::vector<GLfloat> uvs = {};

	Mesh mesh;	
	void addBlockFaceToMesh(int x, int y, int z, BlockFace face);
	void addVisibleBlockFacesToMesh(int x, int y, int z);


public:
	Chunk(glm::vec3 position);
	//~Chunk();

	Block getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, Block block);

	glm::vec3 getPosition();
	void setPosition(glm::vec3 position);

	// get world space coords of chunk mesh vertices
	std::vector<glm::vec3> getVertexCoords();

	void render();
	void generateMesh();
};

#endif