#include "world.h"
#include "biome.h"
#include "render/shader.h"
#include <iostream>
#include "debug.h"

World::World() {
	// prepare RENDER_DISTANCE * RENDER_DISTANCE chunks
	chunks_to_render = new glm::vec3[RENDER_DISTANCE * RENDER_DISTANCE * 4 * MAX_HEIGHT / CHUNK_DIM];
	for (int x = 0; x < RENDER_DISTANCE * 2; x++) {
		for (int z = 0; z < RENDER_DISTANCE * 2; z++) {
			// height map position determined by x-z coords
			glm::vec3 position = glm::vec3(x * CHUNK_DIM, 0.0f, z * CHUNK_DIM);
			GLshort* height_map_ptr = Biome::generateHeightMap(position);
			GLshort* moisture_map_ptr = Biome::generateMoistureMap(position);

			for (int y = 0; y < MAX_HEIGHT / CHUNK_DIM; y++) {
				// add chunk to world
				glm::vec3 chunk_position = position + glm::vec3(0.0f, y * CHUNK_DIM, 0.0f);
				auto chunk = std::make_shared<Chunk>(chunk_position, height_map_ptr, moisture_map_ptr);
				chunks.insert({ {chunk_position, chunk} });

				// Flat[x + WIDTH * (y + DEPTH * z)] = Original[x, y, z]
				chunks_to_render[x + RENDER_DISTANCE * 2 * (y + MAX_HEIGHT / CHUNK_DIM * z)] = chunk_position;

				// only need to generate meshes once since we don't change the mesh at all for now...
				// todo: allow player to interact with world
				chunk->generateMesh();
			}
		}
	}

	// bind 
	block_shader = LoadShaders("../../../../Clonecraft/shaders/block.vert", "../../../../Clonecraft/shaders/block.frag");
	water_shader = LoadShaders("../../../../Clonecraft/shaders/water.vert", "../../../../Clonecraft/shaders/water.frag");
	glCheckError();

	// shader uniforms
	block_mvp_uniform = glGetUniformLocation(block_shader, "mvp");
	water_mvp_uniform = glGetUniformLocation(water_shader, "mvp");
	glCheckError();
}

void World::render(Frustum frustum) {
	// render all solid blocks first
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
				std::shared_ptr<Chunk> chunk_ptr = map_item->second;
				glm::mat4 mvp = frustum.getProjection() * frustum.getView();

				// render solid blocks
				glUseProgram(block_shader);
				glUniformMatrix4fv(block_mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
				chunk_ptr->renderSolidBlocks();
				glUseProgram(0);
			}
		}
	}
	// then render all water blocks
	for (int i = 0; i < RENDER_DISTANCE * RENDER_DISTANCE * 4 * MAX_HEIGHT / CHUNK_DIM; i++) {
		glm::vec3 chunk_coord = chunks_to_render[i];

		// find chunk in the unordered_map
		std::unordered_map<glm::vec3, std::shared_ptr<Chunk>>::const_iterator map_item = chunks.find(chunk_coord);

		if (map_item == chunks.end()) {
			std::cout << "Error: (World) chunk not found" << std::endl;
		}
		else {
			// check if chunk is inside frustum
			std::vector<glm::vec3> vertex_coords = map_item->second->getVertexCoords();
			if (frustum.cubeIntersection(vertex_coords)) {
				std::shared_ptr<Chunk> chunk_ptr = map_item->second;
				glm::mat4 mvp = frustum.getProjection() * frustum.getView();

				// render water
				glUseProgram(water_shader);
				glUniformMatrix4fv(water_mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
				chunk_ptr->renderWater();
				glUseProgram(0);
			}
		}
	}
}