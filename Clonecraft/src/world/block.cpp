#include "block.h"

Block::Block(void) = default;

Block::Block(BlockType type) {
	this->type = type;
}

bool Block::isVisible() {
    return !(type == BlockType::AIR);
}