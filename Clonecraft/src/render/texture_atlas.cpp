#include "texture_atlas.h"

TextureAtlas::TextureAtlas(const char* file_path) {
    texture = Texture(file_path);
}

void TextureAtlas::bind() {
    texture.bind();
}

void TextureAtlas::unbind() {
    texture.unbind();
}

void TextureAtlas::cleanup() {
    texture.cleanup();
}

std::vector<float> TextureAtlas::getAtlasOffsets(BlockType blockType) {
    // [sides_x, sides_y, top_x, top_y, bot_x, bot_y]
    std::vector<float> atlas_offsets;
   
    switch (blockType) {
        case BlockType::GRASS:
            atlas_offsets = { 2, 0, 0, 0, 1, 0 };
            break;
        case BlockType::DIRT:
            atlas_offsets = { 1, 0, 1, 0, 1, 0 };
            break;
        case BlockType::SAND:
            atlas_offsets = { 3, 0, 3, 0, 3, 0 };
            break;
    }
    return atlas_offsets;
}

std::vector<GLfloat> TextureAtlas::getUVs(BlockType blockType) {
    std::vector<float> atlas_offsets = TextureAtlas::getAtlasOffsets(blockType);

    GLfloat block_size = (GLfloat) TEXTURE_SIZE / (GLfloat) ATLAS_SIZE;

    // sides
    GLfloat u_offset = atlas_offsets[0] * TEXTURE_SIZE / ATLAS_SIZE;
    GLfloat v_offset = atlas_offsets[1] * TEXTURE_SIZE / ATLAS_SIZE;

    GLfloat top_left_x = 0.0f + u_offset;
    GLfloat top_left_y = 1.0f - v_offset;
    GLfloat top_right_x = block_size + u_offset;
    GLfloat top_right_y = 1.0f - v_offset;
    GLfloat bot_left_x = 0.0f + u_offset;
    GLfloat bot_left_y = 1.0f - block_size - v_offset;
    GLfloat bot_right_x = block_size + u_offset;
    GLfloat bot_right_y = 1.0f - block_size - v_offset;

    // top 
    u_offset = atlas_offsets[2] * TEXTURE_SIZE / ATLAS_SIZE;
    v_offset = atlas_offsets[3] * TEXTURE_SIZE / ATLAS_SIZE;

    GLfloat ttop_left_x = 0.0f + u_offset;
    GLfloat ttop_left_y = 1.0f - v_offset;
    GLfloat ttop_right_x = block_size + u_offset;
    GLfloat ttop_right_y = 1.0f - v_offset;
    GLfloat tbot_left_x = 0.0f + u_offset;
    GLfloat tbot_left_y = 1.0f - block_size - v_offset;
    GLfloat tbot_right_x = block_size + u_offset;
    GLfloat tbot_right_y = 1.0f - block_size - v_offset;

    // bot 
    u_offset = atlas_offsets[4] * TEXTURE_SIZE / ATLAS_SIZE;
    v_offset = atlas_offsets[5] * TEXTURE_SIZE / ATLAS_SIZE;

    GLfloat btop_left_x = 0.0f + u_offset;
    GLfloat btop_left_y = 1.0f - v_offset;
    GLfloat btop_right_x = block_size + u_offset;
    GLfloat btop_right_y = 1.0f - v_offset;
    GLfloat bbot_left_x = 0.0f + u_offset;
    GLfloat bbot_left_y = 1.0f - block_size - v_offset;
    GLfloat bbot_right_x = block_size + u_offset;
    GLfloat bbot_right_y = 1.0f - block_size - v_offset;

    std::vector<GLfloat> uvs {
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

    return uvs;
}