#include "world.h"
#include "biome.h"
#include <iostream>

World::World() {
	// prepare RENDER_DISTANCE * RENDER_DISTANCE chunks
	chunks_to_render = new glm::vec3[RENDER_DISTANCE * RENDER_DISTANCE * 4 * MAX_HEIGHT / CHUNK_DIM];
	for (int x = 0; x < RENDER_DISTANCE * 2; x++) {
		for (int z = 0; z < RENDER_DISTANCE * 2; z++) {
			glm::vec3 position = glm::vec3(x * CHUNK_DIM, 0.0f, z * CHUNK_DIM);
			GLubyte* height_map = Biome::generateHeightMap(position);

			for (int y = 0; y < MAX_HEIGHT / CHUNK_DIM; y++) {
				// add chunk to world
				position = glm::vec3(x * CHUNK_DIM, y, z * CHUNK_DIM);
				auto chunk = std::make_shared<Chunk>(position, height_map);
				chunks.insert({ {position, chunk} });

				// Flat[x + WIDTH * (y + DEPTH * z)] = Original[x, y, z]
				chunks_to_render[x + RENDER_DISTANCE * 2 * (y + (int) MAX_HEIGHT / CHUNK_DIM * z)] = position;

				// only need to generate meshes once since we don't change the mesh at all for now...
				// todo: allow player to interact with world
				chunk->generateMesh();
			}
		}
	}
}


void World::render(Frustum frustum) {
	for (int i = 0; i < RENDER_DISTANCE * RENDER_DISTANCE * 4 * MAX_HEIGHT / CHUNK_DIM; i++) {
		glm::vec3 chunk_coord = chunks_to_render[i];
		
		// find chunk in the unordered_map
		std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>::const_iterator map_item = chunks.find(chunk_coord);

		if (map_item == chunks.end()) {
			std::cout << "Error: (World) chunk not found" << std::endl;
		} else {
			// check if chunk is inside frustum
			std::vector<glm::vec3> vertex_coords = map_item->second->getVertexCoords();
			if (frustum.cubeIntersection(vertex_coords)) {
				map_item->second->render();
			}
		}
	}
}