#include "world.h"
#include <iostream>

World::World() {
	// prepare RENDER_DISTANCE * RENDER_DISTANCE chunks
	chunks_to_render = new glm::vec3[RENDER_DISTANCE * RENDER_DISTANCE];
	for (int x = 0; x < RENDER_DISTANCE; x++) {
		for (int z = 0; z < RENDER_DISTANCE; z++) {
			// for now only add chunks horizontally
			// todo: add vertical chunking
			glm::vec3 position = glm::vec3(x * CHUNK_DIM, 0.0f, z * CHUNK_DIM);
			auto chunk = std::make_shared<Chunk>(position);
			chunks.insert({ {position, chunk} });
			chunks_to_render[x + RENDER_DISTANCE * z] = position;

			// only need to generate meshes once since we don't change the mesh at all for now...
			chunk->generateMesh();
		}
	}
}

void World::render(Frustum frustum) {
	for (int i = 0; i < RENDER_DISTANCE * RENDER_DISTANCE; i++) {
		glm::vec3 chunk_coord = chunks_to_render[i];
		
		// find chunk in the unordered_map
		std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>::const_iterator map_item = chunks.find(chunk_coord);

		if (map_item == chunks.end()) {
			std::cout << "Error: (World) chunk not found" << std::endl;
		} else {
			// check if chunk is inside frustum
			glm::vec3 pos = map_item->second->getPosition();
			//if (frustum.pointTest(pos))
				map_item->second->render();
		}
	}
}