#include "block.h"

Block::Block(void) = default;

Block::Block(BlockType type) {
	this->type = type;

    Texture texture("../Clonecraft/resources/atlas.png");
    this->texture = texture;
}

Block::Block(BlockType type, glm::vec3 position) {
	this->type = type;
	this->position = position;

    Texture texture("../Clonecraft/resources/atlas.png");
    this->texture = texture;
}

Mesh Block::generateMesh() {
    // need to repeat some coords since faces have different textures
    std::vector<GLfloat> vertices{
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

    std::vector<GLuint> indices{
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


    std::vector<GLfloat> uvs{
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

    // create mesh
    Mesh mesh(vertices, uvs, indices);

    // clean up vectors
    vertices.clear();
    uvs.clear();
    indices.clear();

    return mesh;
}

void Block::render() {
    texture.bind();
    Mesh mesh = generateMesh();
    mesh.render();
    mesh.cleanup();
}

glm::vec3 Block::getPosition() {
    return position;
}

bool Block::isVisible() {
    return !(type == BlockType::AIR);
}