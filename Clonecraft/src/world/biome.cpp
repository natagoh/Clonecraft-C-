#include "FastNoiseLite.h"
#include "world/chunk.h"
#include "world/biome.h"

#include <iostream>

GLshort* Biome::generateHeightMap(glm::vec3 position) {
	// simplex noise [-1, 1]
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	float amplitude[] = {
		1.0f, 0.5f, 0.25f,
	};

	// get height from noise function
	int height_limit = 64; // can be different from world limit
	GLshort* height_map;
	height_map = new GLshort[CHUNK_DIM * CHUNK_DIM];
	//std::cout << "====================" << std::endl; 
	for (int x = 0; x < CHUNK_DIM; x++) {
		for (int z = 0; z < CHUNK_DIM; z++) {
			int world_x = x + position.x;
			int world_z = z + position.z;

			float height = 0;
			float amplitude_sum = 0;
			for (int i = 0; i < sizeof(amplitude) / sizeof(amplitude[0]); i++) {
				noise.SetSeed(i + 666);
				height += amplitude[i] * (noise.GetNoise((float)world_x * 1.0f / amplitude[i], (float)world_z * 1.0f / amplitude[i]) + 1.0f) / 2.0f;
				amplitude_sum += amplitude[i];
			}
			height /= amplitude_sum;

			//float height = (noise.GetNoise((float) world_x, (float) world_z) + 1.0f) / 2.0f;

			height_map[x + CHUNK_DIM * z] = height * (float) height_limit;
			//std::cout << height_map[x + CHUNK_DIM * z] << " ";
		}
		//std::cout << std::endl;
	}

	return height_map;
}

BlockType Biome::getBlockType(int elevation) {
	int height_limit = 64; // todo: make this biome specific
	if (elevation <= (float)height_limit * 0.3)
		return BlockType::WATER;
	else if (elevation <= (float)height_limit * 0.35)
		return BlockType::SAND;
	else if (elevation <= (float)height_limit * 0.5)
		return BlockType::GRASS;
	else if (elevation <= (float)height_limit * 0.7)
		return BlockType::STONE;
	else 
		return BlockType::SNOW;
}
