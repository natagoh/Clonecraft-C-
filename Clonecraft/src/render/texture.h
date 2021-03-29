#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>

class Texture {
private:
	GLuint textureId;

public:
	Texture(const char* file_path);
	Texture();

	GLuint getId();
	void bind();
	void unbind();
	void cleanup();
};

#endif
