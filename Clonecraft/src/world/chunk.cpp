#include "render/texture_atlas.h"
#include "world/world.h"
#include "world/chunk.h"
#include "world/block.h"
#include "world/biome.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <stdlib.h>

const GLfloat Chunk::base_vertices[] = {
    // front
    -0.5, 0.5, 0.5,		// V0
    -0.5, -0.5, 0.5,	// V1
    0.5, -0.5, 0.5,		// V1
    0.5, 0.5, 0.5,		// V3
    // back
    -0.5, 0.5, -0.5,	// V4
    0.5, 0.5, -0.5,		// V5
    -0.5, -0.5, -0.5,	// V6
    0.5, -0.5, -0.5,	// V7
    // right
    0.5, 0.5, 0.5,		// V8: V3 repeated
    0.5, -0.5, 0.5,		// V9: V2 repeated
    0.5, -0.5, -0.5,	// V10: V7 repeated
    0.5, 0.5, -0.5,		// V11: V5 repeated
    // left
    -0.5, 0.5, -0.5,	// V12: V4 repeated
    -0.5, -0.5, -0.5,	// V13: V6 repeated
     -0.5, 0.5, 0.5,	// V14: V0 repeated
    -0.5, -0.5, 0.5,	// V15: V1 repeated
    // top
    -0.5, 0.5, -0.5,	// V16: V4 repeated
    0.5, 0.5, -0.5,		// V17: V5 repeated
    -0.5, 0.5, 0.5,		// V18: V0 repeated
    0.5, 0.5, 0.5,		// V19: V3 repeated
    // bottom
    -0.5, -0.5, -0.5,	// V20: V6 repeated
    0.5, -0.5, -0.5,	// V21: V7 repeated
    -0.5, -0.5, 0.5,	// V22: V1 repeated
    0.5, -0.5, 0.5,		// V23: V2 repeated
};

const GLfloat Chunk::base_normals[] = {
    0.0, 0.0, 1.0,  // front
    0.0, 0.0, -1.0, // back
    1.0, 0.0, 0.0,  // right
    -1.0, 0.0, 0.0, // left
    0.0, 1.0, 0.0,  // top
    0.0, -1.0, 0.0, // bottom
};

const GLushort Chunk::base_indices[] = {
    0, 1, 3, 3, 1, 2,   // front
    0, 3, 2, 1, 3, 0,	// back
    0, 1, 2, 3, 0, 2,   // right
    0, 1, 2, 2, 1, 3,	// left
    0, 2, 3, 1, 0, 3,	// top
    0, 3, 2, 1, 3, 0,   // bottom
};

// constructor
Chunk::Chunk(glm::vec3 position, GLshort* height_map) {
    this->position = position;
    this->height_map = height_map;

    //std::cout << "chunk position" << glm::to_string(position) << std::endl;

    // generating block array
    for (int x = 0; x < CHUNK_DIM; x++) {
        for (int z = 0; z < CHUNK_DIM; z++) {
            int height = height_map[x + CHUNK_DIM * z];
            //std::cout << "height" << height << std::endl;std::cout << "height" << height << std::endl;
            for (int y = 0; y < CHUNK_DIM; y++) {
                int world_y = position.y + y;
                //std::cout << "height: " << height << " world y: " << world_y << std::endl;
                setBlock(x, y, z, Biome::getBlockType(world_y, height));
            }
        }

    }
    
    //// encode block array 
    //// we can store block face data 16*16 in a single int
    //std::vector<GLubyte> rle[CHUNK_DIM];
    //for (int y = 0; y < CHUNK_DIM; y++) {
    //    std::vector<GLubyte> data;
    //    for (int x = 0; x < CHUNK_DIM; x++) {
    //        for (int z = 0; z < CHUNK_DIM; z++) {
    //            BlockType blocktype = getBlock(x, y, z).getType();  // 8 bits
    //            GLubyte count = 1;  // 8 bits
    //            while (z < CHUNK_DIM - 1 && blocktype == getBlock(x, y, z + 1).getType()) {
    //                count++;
    //                z++;
    //            }
    //            data.push_back(blocktype);
    //            data.push_back(count);
    //        }
    //    }
    //    rle[y] = data;
    //}

    //// build interval tree
    //for (int y = 0; y < CHUNK_DIM; y++) {
    //    int idx = 0;
    //    for (int i = 0; i < rle[y].size() / 2; i += 2) {
    //        GLubyte blocktype = rle[y][i];
    //        GLubyte count = rle[y][i + 1];
    //        int start_idx = idx + CHUNK_DIM * y;
    //        int end_idx = start_idx + count;

    //        idx++;
    //    }
    //}


    //std::cout << sizeof(blocks) << " " << sizeof(rle) << std::endl;

  /*  for (int j = 0; j < CHUNK_DIM; j++) {
        std::cout << "=================CHUNK===================" << std::endl;
        for (int i = 0; i < CHUNK_DIM; i++) {
            for (int k = 0; k < CHUNK_DIM; k++) {
                std::cout << static_cast<unsigned>(getBlock(i, j, k).getType()) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "=================CHUNK ENCODING===================" << std::endl;
        for (int k = 0; k < rle[j].size(); k++) {
            std::cout << static_cast<unsigned>(rle[j][k]) << " ";
        }
        std::cout << std::endl;
    }*/
}

// get world space coords of chunk vertices (for frustum culling)
std::vector<glm::vec3> Chunk::getVertexCoords() {
    std::vector<glm::vec3> vertex_coords;

    vertex_coords.push_back(position);  
    vertex_coords.push_back(position + glm::vec3(CHUNK_DIM, 0, 0));  
    vertex_coords.push_back(position + glm::vec3(0, 0, CHUNK_DIM));
    vertex_coords.push_back(position + glm::vec3(CHUNK_DIM, 0, CHUNK_DIM));

    vertex_coords.push_back(position + glm::vec3(0, CHUNK_DIM, 0));
    vertex_coords.push_back(position + glm::vec3(CHUNK_DIM, CHUNK_DIM, 0));
    vertex_coords.push_back(position + glm::vec3(0, CHUNK_DIM, CHUNK_DIM));
    vertex_coords.push_back(position + glm::vec3(CHUNK_DIM, CHUNK_DIM, CHUNK_DIM));

    return vertex_coords;
}

// get the block at the specified chunk coordinates
Block Chunk::getBlock(int x, int y, int z) {
	return blocks[x + CHUNK_DIM * (y + CHUNK_DIM * z)];
}

// set block at specified chunk coordinates
void Chunk::setBlock(int x, int y, int z, Block block) {
    blocks[x + CHUNK_DIM * (y + CHUNK_DIM * z)] = block;
}

// get the chunk's position in game world coords
glm::vec3 Chunk::getPosition() {
    return position;
}

// set the chunk's position in game world coords
void Chunk::setPosition(glm::vec3 position) {
    this->position = position;
}

// make sure to call generateMesh at least once before render
void Chunk::renderSolidBlocks() {
    mesh.render();
}

void Chunk::renderWater() {
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    water_mesh.render();
}

void Chunk::generateMesh() {
    // decompress block data

	for (int x = 0; x < CHUNK_DIM; x++) {
		for (int y = 0; y < CHUNK_DIM; y++) {
			for (int z = 0; z < CHUNK_DIM; z++) {
                if (getBlock(x, y, z).getType() == BlockType::WATER) {
                    addVisibleWaterFacesToMesh(x, y, z);
                } else {
                    addVisibleBlockFacesToMesh(x, y, z);
                }
                
			}
		}
	}

    // solid blocks mesh
    mesh = Mesh(vertices, uvs, normals, indices); 
    water_mesh = Mesh(water_vertices, water_uvs, water_normals, water_indices);

    // clean up buffer vectors once data already pushed to mesh
    vertices.clear();
    uvs.clear();
    indices.clear();
    normals.clear();

    water_vertices.clear();
    water_uvs.clear();
    water_indices.clear();
    water_normals.clear();
}

// add visible block faces to mesh
void Chunk::addVisibleBlockFacesToMesh(int x, int y, int z) {
    // don't add AIR blocks
    if (getBlock(x, y, z).getType() == BlockType::AIR) return;

    // render face if block is not water, but neighbor is WATER
    if (x == 0 || !getBlock(x - 1, y, z).isVisible() || getBlock(x - 1, y, z).getType() == BlockType::WATER) {
        addFaceToMesh(x, y, z, Face::LEFT, MeshType::SOLID_BLOCK);
    }
    if (x == CHUNK_DIM - 1 || !getBlock(x + 1, y, z).isVisible() || getBlock(x + 1, y, z).getType() == BlockType::WATER) {
        addFaceToMesh(x, y, z, Face::RIGHT, MeshType::SOLID_BLOCK);
    }
    if (y == 0 || !getBlock(x,  y - 1, z).isVisible()) {
        addFaceToMesh(x, y, z, Face::BOTTOM, MeshType::SOLID_BLOCK);
    }
    if (y == CHUNK_DIM - 1 || !getBlock(x, y + 1, z).isVisible() || getBlock(x, y + 1, z).getType() == BlockType::WATER) {
        addFaceToMesh(x, y, z, Face::TOP, MeshType::SOLID_BLOCK);
    }
    if (z == 0 || !getBlock(x, y, z - 1).isVisible() || getBlock(x, y, z - 1).getType() == BlockType::WATER) {
        addFaceToMesh(x, y, z, Face::BACK, MeshType::SOLID_BLOCK);
    }
    if (z == CHUNK_DIM - 1 || !getBlock(x, y, z + 1).isVisible() || getBlock(x, y, z + 1).getType() == BlockType::WATER) {
        addFaceToMesh(x, y, z, Face::FRONT, MeshType::SOLID_BLOCK);
    }
}

// add visible water block faces to mesh
void Chunk::addVisibleWaterFacesToMesh(int x, int y, int z) {
    if (y == CHUNK_DIM - 1 || !getBlock(x, y + 1, z).isVisible()) {
        addFaceToMesh(x, y, z, Face::TOP, MeshType::WATER);
    }
}

// add the block face at x, y, z to the chunk's mesh
void Chunk::addFaceToMesh(int x, int y, int z, Face face, MeshType meshType) {
    BlockType blocktype = getBlock(x, y, z).getType();
    if (blocktype == BlockType::AIR) {
        std::cout << "Error: (Chunk) AIR block face should not be added to mesh" << std::endl;
    }

    std::vector<GLfloat>* mesh_vertices = &vertices;
    std::vector<GLuint>* mesh_indices = &indices;
    std::vector<GLfloat>* mesh_uvs = &uvs;
    std::vector<GLfloat>* mesh_normals = &normals;

    if (meshType == MeshType::WATER) {
        mesh_vertices = &water_vertices;
        mesh_indices = &water_indices;
        mesh_uvs = &water_uvs;
        mesh_normals = &water_normals;
    }
   
    // index of block we are adding
    unsigned int num_points = mesh_vertices->size() / 3;
    unsigned int face_index = num_points / NUM_POINTS_PER_FACE;

    // add the position-offset vertices of the newly added block face
    // for water, make block slightly shorter
    float water_height_offset = 1.0f / 16.0f;
    unsigned int start_idx = FACE_VERTICES_OFFSET * face;
    for (unsigned int i = 0; i < NUM_POINTS_PER_FACE; i++) {
        mesh_vertices->push_back(base_vertices[start_idx + i * 3] + position.x + x);
        if (blocktype == BlockType::WATER && base_vertices[start_idx + i * 3 + 1] > 0) {
            mesh_vertices->push_back(base_vertices[start_idx + i * 3 + 1] + position.y + y - water_height_offset);
        } else {
            mesh_vertices->push_back(base_vertices[start_idx + i * 3 + 1] + position.y + y);
        }
        mesh_vertices->push_back(base_vertices[start_idx + i * 3 + 2] + position.z + z);
    }

    // add the indices of the newly added block face
    start_idx = FACE_INDICES_OFFSET * face;
    unsigned int end_idx = start_idx + FACE_INDICES_OFFSET;
    for (unsigned int i = start_idx; i < end_idx; i++) {
        mesh_indices->push_back(base_indices[i] + NUM_POINTS_PER_FACE * face_index);
    }

    // get uvs from texture atlas
    std::vector<GLfloat> texture_uvs = TextureAtlas::getUVs(getBlock(x, y, z).getType());
    start_idx = FACE_UV_OFFSET * face;
    for (unsigned int i = 0; i < NUM_POINTS_PER_FACE; i++) {
        mesh_uvs->push_back(texture_uvs[start_idx + i * 2]);
        mesh_uvs->push_back(texture_uvs[start_idx + i * 2 + 1]);
    }

    // put in normals
    start_idx = FACE_NORMAL_OFFSET * face;
    for (int j = 0; j < NUM_POINTS_PER_FACE; j++) {
        mesh_normals->push_back(base_normals[start_idx]);
        mesh_normals->push_back(base_normals[start_idx + 1]);
        mesh_normals->push_back(base_normals[start_idx + 2]);
    }
}