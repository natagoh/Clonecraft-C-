#include "mesh.h"
#include <iostream>

Mesh::Mesh(void) = default;

Mesh::Mesh(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices) {
    num_vertices = vertices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertices vbo
    glGenBuffers(2, vbos);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // indices vbo (element buffer)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

Mesh::Mesh(std::vector<GLfloat> &vertices, std::vector<GLfloat> &uvs, std::vector<GLuint> &indices) {
    num_vertices = vertices.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertices vbo
    glGenBuffers(3, vbos);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    // indices vbo (element buffer)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // texture coords vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(GLfloat), uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

}

unsigned int Mesh::getNumVertices() {
	return num_vertices;
}

void Mesh::render() {
    // draw mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, getNumVertices(), GL_UNSIGNED_INT, 0);

    glFlush();
}

// type can be one of
// GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, etc...
// see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawElements.xhtml#:~:text=When%20glDrawElements%20is%20called%2C%20it,is%20enabled%2C%20each%20is%20used.
void Mesh::render(GLenum mode) {
    // draw mesh
    glBindVertexArray(vao);
    glDrawElements(mode, getNumVertices(), GL_UNSIGNED_INT, 0);

    glFlush();
}


void Mesh::cleanup() {
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // delete vbos
    glDeleteBuffers(3, vbos);

    // delete vao
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}