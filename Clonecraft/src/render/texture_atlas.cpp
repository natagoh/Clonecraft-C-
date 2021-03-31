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

std::vector<GLubyte> TextureAtlas::getAtlasOffsets(BlockType blockType) {
    // [sides_x, sides_y, top_x, top_y, bot_x, bot_y]
    std::vector<GLubyte> atlas_offsets;
   
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
    std::vector<GLubyte> atlas_offsets = TextureAtlas::getAtlasOffsets(blockType);

    GLfloat block_size = (GLfloat) TEXTURE_SIZE / (GLfloat) ATLAS_SIZE;

    // sides
    GLfloat u_offset = (float) atlas_offsets[0] * block_size;
    GLfloat v_offset = (float) atlas_offsets[1] * block_size;

    glm::vec2 bl = glm::vec2(0.0f + u_offset, 1.0f - block_size - v_offset);
    glm::vec2 br = glm::vec2(block_size + u_offset, 1.0f - block_size - v_offset);
    glm::vec2 tl = glm::vec2(0.0f + u_offset, 1.0f - v_offset);
    glm::vec2 tr = glm::vec2(block_size + u_offset, 1.0f - v_offset);

    // top 
    u_offset = (float) atlas_offsets[2] * TEXTURE_SIZE / ATLAS_SIZE;
    v_offset = (float) atlas_offsets[3] * TEXTURE_SIZE / ATLAS_SIZE;

    glm::vec2 tbl = glm::vec2(0.0f + u_offset, 1.0f - block_size - v_offset);
    glm::vec2 tbr = glm::vec2(block_size + u_offset, 1.0f - block_size - v_offset);
    glm::vec2 ttl = glm::vec2(0.0f + u_offset, 1.0f - v_offset);
    glm::vec2 ttr = glm::vec2(block_size + u_offset, 1.0f - v_offset);

    // bot 
    u_offset = (float) atlas_offsets[4] * TEXTURE_SIZE / ATLAS_SIZE;
    v_offset = (float) atlas_offsets[5] * TEXTURE_SIZE / ATLAS_SIZE;

    glm::vec2 bbl = glm::vec2(0.0f + u_offset, 1.0f - block_size - v_offset);
    glm::vec2 bbr = glm::vec2(block_size + u_offset, 1.0f - block_size - v_offset);
    glm::vec2 btl = glm::vec2(0.0f + u_offset, 1.0f - v_offset);
    glm::vec2 btr = glm::vec2(block_size + u_offset, 1.0f - v_offset);

    std::vector<GLfloat> uvs {
        // front
        tl.x, tl.y,
        bl.x, bl.y,
        br.x, br.y,
        tr.x, tr.y,
        // back
        tl.x, tl.y,
        tr.x, tr.y,
        bl.x, bl.y,
        br.x, br.y,
        // right
        tl.x, tl.y,
        bl.x, bl.y,
        br.x, br.y,
        tr.x, tr.y,
        // left
        tr.x, tr.y,
        br.x, br.y,
        tl.x, tl.y,
        bl.x, bl.y,
        // top
        ttl.x, ttl.y,
        ttr.x, ttr.y,
        tbl.x, tbl.y,
        tbr.x, tbr.y,
        // bot
        btl.x, btl.y,
        btr.x, btr.y,
        bbl.x, bbl.y,
        bbr.x, bbr.y,
    };

    return uvs;
}