#include "world/chunk.h"
#include <iostream>

Chunk::Chunk(void) {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				blocks[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)] = Block(BlockType::GRASS);
			}
		}
	}

}

// get the block at the specified chunk coordinates
Block Chunk::getBlock(int x, int y, int z) {
	return blocks[x + CHUNK_SIZE * (y + CHUNK_SIZE * z)];
}

// is a block in the chunk visible at this coord?
bool Chunk::blockIsVisibleAt(int x, int y, int z) {
	// check edge of chunk
	bool x_in_range = x > 0 && x < CHUNK_SIZE - 1;
	bool y_in_range = y > 0 && y < CHUNK_SIZE - 1;
	bool z_in_range = z > 0 && z < CHUNK_SIZE - 1;
	if (!x_in_range || !y_in_range || !z_in_range) {
		return true;
	}

	// check if neighboring blocks exist
	if (getBlock(x + 1, y, z).isVisible() && getBlock(x - 1, y, z).isVisible() &&
		getBlock(x, y + 1, z).isVisible() && getBlock(x, y - 1, z).isVisible() &&
		getBlock(x, y, z + 1).isVisible() && getBlock(x, y, z - 1).isVisible()) {
		return false;
	}

	return true;
}

void Chunk::render() {
    Mesh mesh = generateMesh();
    mesh.render();
    mesh.cleanup();
    mesh.render();
}

// todo implement this function
Mesh Chunk::generateMesh() {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				if (blockIsVisibleAt(x, y, z)) {
                    addBlockToMesh(x, y, z);
				}
			}
		}
	}
    return Mesh(vertices, uvs, indices);
}

// add the block at x, y, z to the chunk's mesh
void Chunk::addBlockToMesh(int x, int y, int z) {
    std::vector<GLfloat> base_vertices {
        // front
        -0.5f, 0.5f, 0.5f,		// V0
        -0.5f, -0.5f, 0.5f,		// V1
        0.5f, -0.5f, 0.5f,		// V1
        0.5f, 0.5f, 0.5f,		// V3
        // back
        -0.5f, 0.5f, -0.5f,		// V4
        0.5f, 0.5f, -0.5f,		// V5
        -0.5f, -0.5f, -0.5f,	// V6
        0.5f, -0.5f, -0.5f,		// V7
        // right
        0.5f, 0.5f, 0.5f,		// V8: V3 repeated
        0.5f, -0.5f, 0.5f,		// V9: V2 repeated
        // left
        -0.5f, 0.5f, -0.5f,		// V10: V4 repeated
        -0.5f, -0.5f, -0.5f,	// V11: V6 repeated
        // top
        -0.5f, 0.5f, -0.5f,		// V12: V4 repeated
        0.5f, 0.5f, -0.5f,		// V13: V5 repeated
        -0.5f, 0.5f, 0.5f,		// V14: V0 repeated
        0.5f, 0.5f, 0.5f,		// V15: V3 repeated
        // bot
        -0.5f, -0.5f, -0.5f,	// V16: V6 repeated
        0.5f, -0.5f, -0.5f,		// V17: V7 repeated
        -0.5f, -0.5f, 0.5f,		// V18: V1 repeated
        0.5f, -0.5f, 0.5f,		// V19: V2 repeated

    };

    std::vector<GLuint> base_indices {
        0, 1, 3, 3, 1, 2,		// front
        4, 7, 6, 5, 7, 4,		// back
        8, 9, 7, 5, 8, 7,	    // right
        10, 11, 0, 0, 11, 1,	// left
        12, 14, 15, 13, 12, 15,	// top
        16, 19, 18, 17, 19, 16, // bot
    };

    GLfloat texture_size = 16.0f;
    GLfloat atlas_size = 256.0f;

    GLfloat block_size = texture_size / atlas_size;

    // sides
    GLfloat u_offset = 2 * texture_size / atlas_size;
    GLfloat v_offset = 0 * texture_size / atlas_size;

    GLfloat top_left_x = 0.0f + u_offset;
    GLfloat top_left_y = 1.0f - v_offset;
    GLfloat top_right_x = block_size + u_offset;
    GLfloat top_right_y = 1.0f - v_offset;
    GLfloat bot_left_x = 0.0f + u_offset;
    GLfloat bot_left_y = 1.0f - block_size - v_offset;
    GLfloat bot_right_x = block_size + u_offset;
    GLfloat bot_right_y = 1.0f - block_size - v_offset;

    // top 
    u_offset = 0 * texture_size / atlas_size;
    v_offset = 0 * texture_size / atlas_size;

    GLfloat ttop_left_x = 0.0f + u_offset;
    GLfloat ttop_left_y = 1.0f - v_offset;
    GLfloat ttop_right_x = block_size + u_offset;
    GLfloat ttop_right_y = 1.0f - v_offset;
    GLfloat tbot_left_x = 0.0f + u_offset;
    GLfloat tbot_left_y = 1.0f - block_size - v_offset;
    GLfloat tbot_right_x = block_size + u_offset;
    GLfloat tbot_right_y = 1.0f - block_size - v_offset;

    // bot 
    u_offset = 1 * texture_size / atlas_size;
    v_offset = 0 * texture_size / atlas_size;

    GLfloat btop_left_x = 0.0f + u_offset;
    GLfloat btop_left_y = 1.0f - v_offset;
    GLfloat btop_right_x = block_size + u_offset;
    GLfloat btop_right_y = 1.0f - v_offset;
    GLfloat bbot_left_x = 0.0f + u_offset;
    GLfloat bbot_left_y = 1.0f - block_size - v_offset;
    GLfloat bbot_right_x = block_size + u_offset;
    GLfloat bbot_right_y = 1.0f - block_size - v_offset;


    std::vector<GLfloat> base_uvs {
        // front
        top_left_x, top_left_y,
        bot_left_x, bot_left_y,
        bot_right_x, bot_right_y,
        top_right_x, top_right_y,
        // back
        top_left_x, top_left_y,
        top_right_x, top_right_y,
        bot_left_x, bot_left_y,
        bot_right_x, bot_right_y,
        // right
        top_left_x, top_left_y,
        bot_left_x, bot_left_y,
        // left
        top_right_x, top_right_y,
        bot_right_x, bot_right_y,
        // top
        ttop_left_x, ttop_left_y,
        ttop_right_x, ttop_right_y,
        tbot_left_x, tbot_left_y,
        tbot_right_x, tbot_right_y,
        // bot
        btop_left_x, btop_left_y,
        btop_right_x, btop_right_y,
        bbot_left_x, bbot_left_y,
        bbot_right_x, bbot_right_y,
    };

    // should be 20
    unsigned int num_vertex_coords = base_vertices.size() / 3;

    // add the position-offset vertices of the newly added block
    for (size_t i = 0; i < num_vertex_coords / 3; i++) {
        vertices.push_back(base_vertices[i] + position.x);
        vertices.push_back(base_vertices[i + 1] + position.y);
        vertices.push_back(base_vertices[i + 2] + position.z);
    }

    // add the indices of the newly added block
    for (size_t i = 0; i < base_indices.size(); i++) {
        indices.push_back(base_indices[i] + num_vertex_coords);
    }

    // for now, all blocks have the same texture
    uvs.insert(std::end(uvs), std::begin(base_uvs), std::end(base_uvs));
}

