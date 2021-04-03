#ifndef WORLD_HPP
#define WORLD_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <glad/glad.h>
#include <memory>
#include <unordered_map>
#include "chunk.h"
#include "render/frustum.h"

#define RENDER_DISTANCE 8	// how many chunks can be seen on a screen in one direction

class World {
private:
	std::unordered_map<glm::vec3, std::shared_ptr<Chunk>> chunks;	// map containing all chunks
	glm::vec3* chunks_to_render;	// array of chunks to be rendered

	GLuint block_shader;
	GLuint water_shader;

	GLuint block_mvp_uniform;
	GLuint water_mvp_uniform;

public:
	static const GLushort MAX_HEIGHT = 64; // ideally 256

	World();
	void render(Frustum frustum);
};

#endif
