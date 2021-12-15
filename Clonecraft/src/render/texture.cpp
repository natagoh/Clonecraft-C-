#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.h"

Texture::Texture(const char* file_path) {
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // set the texture wrapping/filtering options 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // mipmap
    glEnable(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    // load and generate the texture
    int width, height, num_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(file_path, &width, &height, &num_channels, 0);
    if (data) {
        if (num_channels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else if (num_channels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Error: (Texture) unknown number of channels" << std::endl;
        }
    } 
    else {
        std::cout << "Error: (Texture) Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

Texture::Texture() = default;

GLuint Texture::getId() {
    return textureId;
}

void Texture::bind() {
    // set texture sampler to use Texture Unit 0
    glUniform1i(textureId, 0);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::cleanup() {
    glDeleteTextures(1, &textureId);
}