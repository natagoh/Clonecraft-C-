#include "world/chunk.h"

Chunk::Chunk() {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				blocks[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] = Block(BlockType::GRASS);
			}
		}
	}

}

// get the block at the specified chunk coordinates
Block Chunk::getBlockAt(int x, int y, int z) {
	return blocks[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)];
}

// is a block in the chunk visible at this coord?
bool Chunk::isVisibleAt(int x, int y, int z) {
	// check edge of chunk
	bool x_in_range = x > 0 && x < CHUNK_SIZE - 1;
	bool y_in_range = y > 0 && y < CHUNK_SIZE - 1;
	bool z_in_range = z > 0 && z < CHUNK_SIZE - 1;
	if (!x_in_range || !y_in_range || !z_in_range) {
		return true;
	}

	// check if neighboring blocks exist
	if (getBlockAt(x + 1, y, z).isVisible() && getBlockAt(x - 1, y, z).isVisible() &&
		getBlockAt(x, y + 1, z).isVisible() && getBlockAt(x, y - 1, z).isVisible() &&
		getBlockAt(x, y, z + 1).isVisible() && getBlockAt(x, y, z - 1).isVisible()) {
		return false;
	}

	return true;
}


// todo implement this function
void Chunk::generateMesh() {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (isVisibleAt(x, y, z)) {
					return;
				}
			}
		}
	}
}
//
//void Chunk::addBlockToMesh(int x, int y, int z) {
//	
//}

