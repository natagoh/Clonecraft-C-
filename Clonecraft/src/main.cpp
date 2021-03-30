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
#include "render/frustum.h"
#include "input.h"
#include "render/shader.h"
#include "render/texture_atlas.h"
#include "render/mesh.h"
#include "world/chunk.h"
#include "world/world.h"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

GLFWwindow* initWindow();

int main() {
    GLFWwindow* window = initWindow();

    // Check for Valid Context
    if (window == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // camera setup
    glm::vec3 cameraPos = glm::vec3(0.0f, 20.0f, 4.0f);
    glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), mWidth / mHeight, 0.1f, 100.0f);

    Camera camera(cameraPos, cameraDir, cameraUp);

    Frustum frustum(camera, projection);

    // keyboard + mouse input setup
    Input input(window, &camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // texture for the world
    TextureAtlas textureAtlas("../Clonecraft/resources/atlas.png");

    //Chunk chunk = Chunk();
    //chunk.generateMesh();

    World world = World();

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("../Clonecraft/shaders/simple.vert", "../Clonecraft/shaders/simple.frag");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "mvp");
    //GLuint textureID = glGetUniformLocation(programID, "texture_sampler");

    // Rendering Loop
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

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

        glClearColor(47.0f/256.0f, 102.0f/256.0f, 169.0f/256.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // camera/view transformation
        glm::mat4 view = camera.getView();

        // bind texture
        textureAtlas.bind();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = projection * view * model; // Remember, matrix multiplication is the other way around
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
      
        //// render chunk
        //chunk.render();

        // render world
        world.render();

        // render frustum
        //frustum.render();
      
        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
    }   


    // cleanup shader
    glDeleteProgram(programID);

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
    auto window = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

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

    // wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    return window;
}
