#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <vector>

class Mesh {
private:
	GLuint vao;
	GLuint vbos[3];
	unsigned int num_vertices;

public:
	Mesh(std::vector<GLfloat> &vertices, std::vector<GLfloat> &uvs, std::vector<GLuint> &indices);

	GLuint getVao();
	unsigned int getNumVertices();
	void render();
	void cleanup();
};

#endif