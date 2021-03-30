#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <vector>

class Mesh {
private:
	GLuint vao;
	GLuint vbos[3];
	unsigned int num_vertices = 0;

public:
	Mesh(void);
	Mesh(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices);
	Mesh(std::vector<GLfloat> &vertices, std::vector<GLfloat> &uvs, std::vector<GLuint> &indices);

	unsigned int getNumVertices();
	void render();

	void render(GLenum mode);
	void cleanup();
};

#endif