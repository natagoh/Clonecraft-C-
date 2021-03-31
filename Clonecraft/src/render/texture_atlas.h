#ifndef TEXTURE_ATLAS_HPP
#define TEXTURE_ATLAS_HPP

#include <vector>

#include "texture.h"
#include "world/block.h"

class TextureAtlas {
private:
	static const unsigned int TEXTURE_SIZE = 16;				// size of 1 texture in the atlas
	static const unsigned int ATLAS_SIZE = 16 * TEXTURE_SIZE;	// size of entire atlas, atlas is square

	Texture texture;
	
	// get texture atlas coords for a block type
	static std::vector<GLubyte> getAtlasOffsets(BlockType blockType);

public:
	TextureAtlas(const char* file_path);

	void bind();
	void unbind();
	void cleanup();

	static std::vector<GLfloat> getUVs(BlockType blockType);
};

#endif
