#include "world.h"

World::World() {
	// prepare RENDER_DISTANCE * RENDER_DISTANCE chunks
	chunks_to_render = new Chunk[RENDER_DISTANCE * RENDER_DISTANCE];
	for (int x = 0; x < RENDER_DISTANCE; x++) {
		for (int z = 0; z < RENDER_DISTANCE; z++) {
			// for now only add chunks horizontally
			// todo: add vertical chunking
			glm::vec3 position = glm::vec3(x * CHUNK_DIM, 0.0f, z * CHUNK_DIM);
			chunks_to_render[x + RENDER_DISTANCE * z].setPosition(position);
			chunks_to_render[x + RENDER_DISTANCE * z].generateMesh();
		}
	}
}

void World::render() {
	for (int i = 0; i < RENDER_DISTANCE * RENDER_DISTANCE; i++) {
		chunks_to_render[i].render();
	}
}