#include "world.h"
#include "FastNoiseLite.h"

#include <iostream>

World::World() {
	// simplex noise [0, 1]
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	float amplitude[] = {
		1, 0.5, 0.25,
	};


	// prepare RENDER_DISTANCE * RENDER_DISTANCE chunks
	std::vector<GLubyte> height_map;
	chunks_to_render = new glm::vec3[RENDER_DISTANCE * RENDER_DISTANCE * 4];
	for (int x = 0; x < RENDER_DISTANCE * 2; x++) {
		for (int z = 0; z < RENDER_DISTANCE * 2; z++) {
			// for now only add chunks horizontally
			// todo: add vertical chunking
			glm::vec3 position = glm::vec3(x * CHUNK_DIM, 0.0f, z * CHUNK_DIM);
		
			// get height from noise function
			for (int xx = 0; xx < CHUNK_DIM; xx++) {
				for (int zz = 0; zz < CHUNK_DIM; zz++) {
					int world_x = xx + position.x;
					int world_z = zz + position.z;

					float height = 0;
					float amplitude_sum = 0;
					for (int i = 0; i < sizeof(amplitude) / sizeof(amplitude[0]); i++) {
						height += amplitude[i] * noise.GetNoise((float) world_x * 1.0f / amplitude[i], (float) world_z * 1.0f / amplitude[i]);
						amplitude_sum += amplitude[i];
					}
					height /= amplitude_sum;

					/*height_map[xx + CHUNK_DIM * zz] = height * (float) MAX_HEIGHT;*/
					height_map.push_back(height * (float)MAX_HEIGHT);
				}
			}

			// add chunk to world
			auto chunk = std::make_shared<Chunk>(position, height_map);
			chunks.insert({ {position, chunk} });
			chunks_to_render[x + RENDER_DISTANCE * 2 * z] = position;

			height_map.clear();
			
			// only need to generate meshes once since we don't change the mesh at all for now...
			chunk->generateMesh();
		}
	}
}

void World::render(Frustum frustum) {
	for (int i = 0; i < RENDER_DISTANCE * RENDER_DISTANCE * 4; i++) {
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