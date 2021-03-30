#include "block.h"

Block::Block(void) = default;

Block::Block(BlockType type) {
	this->type = type;
}

BlockType Block::getType() {
	return type;
}

bool Block::isVisible() {
    return !(type == BlockType::AIR);
}