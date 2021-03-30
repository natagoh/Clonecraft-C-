#ifndef WORLD_HPP
#define WORLD_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include <memory>
#include <unordered_map>
#include "chunk.h"

class World {
private:
	const unsigned int RENDER_DISTANCE = 4;	// how many chunks can be seen on a screen

	std::unordered_map<glm::vec3, std::shared_ptr<Chunk>> chunks;	// map containing all chunks
	glm::vec3* chunks_to_render;	// array of chunks to be rendered

public:
	World();

	void render();
};

#endif
