#include "glitter.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "camera.h"
#include "input.h"
#include "render/shader.h"
#include "render/texture_atlas.h"
#include "render/frustum.h"
#include "world/world.h"
#include "world/chunk.h"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float m_width, m_height;

GLFWwindow* initWindow();
void setFullscreenResolution();

int main() {
    GLFWwindow* window = initWindow();

    // Check for Valid Context
    if (window == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }
    
    // camera setup
    glm::vec3 camera_pos = glm::vec3(RENDER_DISTANCE * CHUNK_DIM, 20.0f, RENDER_DISTANCE * CHUNK_DIM);
    glm::vec3 camera_dir = glm::vec3(0.0f, -0.8f, 0.2f);
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), m_width / m_height, 0.1f, 1000.0f);

    Camera camera(camera_pos, camera_dir, camera_up);

    Frustum frustum(&camera, projection);
    //frustum.generatePlanes();

    // keyboard + mouse input setup
    Input input(window, &camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // texture for the world
    TextureAtlas textureAtlas("../Clonecraft/resources/atlas.png");

    World world = World();

   /* Chunk chunk = Chunk(glm::vec3(0, 0, 0));
    chunk.generateMesh();*/

    // Create and compile our GLSL program from the shaders
    GLuint frustum_shader = LoadShaders("../Clonecraft/shaders/frustum.vert", "../Clonecraft/shaders/frustum.frag");
    GLuint frustum_mvp_uniform = glGetUniformLocation(frustum_shader, "mvp");

    // Rendering Loop
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // update camera speed
        camera.setSpeed(16 * deltaTime);

        // input
        // -----
        input.handleKeyboard();
        input.handleMouseCursor();

        frustum.generatePlanes();

        glClearColor(47.0f / 256.0f, 102.0f / 256.0f, 169.0f / 256.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// check OpenGL error
        //GLenum err;
        //while ((err = glGetError()) != GL_NO_ERROR) {
        //    std::cerr << "OpenGL error: " << err << std::endl;
        //}

        // bind texture
        textureAtlas.bind();

        // render world
        world.render(frustum);
        //chunk.render();

        //// debug render frustum
        //glm::mat4 mvp = frustum.getProjection() * frustum.getView();
        //glUseProgram(frustum_shader);
        //glUniformMatrix4fv(frustum_mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
        //frustum.render();
        //glUseProgram(0);

        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // cleanup shader
    //glDeleteProgram(block_shader);

    // cleanup texture
    textureAtlas.cleanup();

    glfwTerminate();
    return EXIT_SUCCESS;
}

GLFWwindow* initWindow() {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // MSAA
    glfwWindowHint(GLFW_SAMPLES, 8);

    // double buffering
    glfwWindowHint(GLFW_DOUBLEBUFFER, true);

    // fullscreen
    setFullscreenResolution();

    auto window = glfwCreateWindow(m_width, m_height, "Mycraft", glfwGetPrimaryMonitor(), nullptr);

    // Check for Valid Context
    if (window == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return nullptr;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(window);
    gladLoadGL();

    // backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);

    // MSAA anti aliasing
    glEnable(GL_MULTISAMPLE);

    // wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    return window;
}

// sets the Window's Size to the Primary Monitor's Resolution
void setFullscreenResolution() {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    m_width = mode->width;
    m_height = mode->height;
}
