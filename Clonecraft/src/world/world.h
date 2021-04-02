#ifndef WORLD_HPP
#define WORLD_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <memory>
#include <unordered_map>
#include "chunk.h"
#include "render/frustum.h"

#define RENDER_DISTANCE 1	// how many chunks can be seen on a screen in one direction

class World {
private:
	std::unordered_map<glm::vec3, std::shared_ptr<Chunk>> chunks;	// map containing all chunks
	glm::vec3* chunks_to_render;	// array of chunks to be rendered
	
public:
	static const GLushort MAX_HEIGHT = 64; // ideally 256

	World();
	void render(Frustum frustum);
};

#endif
