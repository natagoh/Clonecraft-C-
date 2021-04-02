#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <vector>

class Mesh {
private:
	GLuint vao;
	GLuint vbos[4];
	unsigned int num_indices = 0;

public:
	Mesh(void);
	Mesh(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
	Mesh(std::vector<GLfloat> &vertices, std::vector<GLfloat> &uvs, std::vector<GLuint> &indices);
	Mesh(std::vector<GLfloat>& vertices, std::vector<GLfloat> &uvs, std::vector<GLfloat> &normals, std::vector<GLuint> &indices);

	unsigned int getNumIndices();
	void render();

	void render(GLenum mode);
	void cleanup();
};

#endif