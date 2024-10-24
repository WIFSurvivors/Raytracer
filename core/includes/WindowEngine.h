#pragma once
#include "ShaderCompiler.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

struct WindowEngine {

  void static framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);
  void static processInput(GLFWwindow *window);

  uint32_t SCR_HEIGHT = 800;
  uint32_t SCR_WIDTH = 800;

  GLFWwindow *window;
  GLuint VBO;
  GLuint VAO;
  GLuint TextureID;

  std::vector<glm::vec3> vertecies = {glm::vec3{-0.5f, -0.5f, 0.0f},
                                      glm::vec3{0.5f, -0.5f, 0.0f},
                                      glm::vec3{0.0f, 0.5f, 0.0f}};

  std::vector<glm::vec2> textureCoords = {
      glm::vec2{0.0f, 0.00f}, glm::vec2{1.0f, 0.0f}, glm::vec2{0.5f, 1.0f}};

  void initializeEngine() {
    if (!initGLFW())
      throw std::runtime_error{"Failed to Initialize Engine"};
    render_loop();
    terminate();
  }

  void terminate() {
    // Do Deletion Here
    glfwTerminate();
  }
  void initTextures() {

    glGenTextures(1, &TextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT,
                 NULL);

    glBindImageTexture(0, TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureID);

  }

  void render_loop() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec3),
                 vertecies.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    initTextures();

    Shader simpleShader{
        std::make_pair(GL_VERTEX_SHADER, "../shaders/vertexshader.glsl"),
        std::make_pair(GL_FRAGMENT_SHADER, "../shaders/fragmentshader.glsl")};
    simpleShader.activateShader();

    glUniform1i(glGetUniformLocation(simpleShader.programID, "text"), 0); // This is the id the texture id the compute shader writes into

    GLuint Tex_VBO;
    glGenBuffers(1, &Tex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Tex_VBO);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2),
                 textureCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(1);

    Shader computeShader{
        std::make_pair(GL_COMPUTE_SHADER, "../shaders/computeshader.glsl")};

    while (!glfwWindowShouldClose(window)) {

      // input
      // -----
      processInput(window);

      // render
      // ------
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      computeShader.activateShader();
      glUniform1f(glGetUniformLocation(computeShader.programID,"time"), glfwGetTime());
      glDispatchCompute(SCR_WIDTH, SCR_HEIGHT, 1);
      glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
      simpleShader.activateShader();
      glDrawArrays(GL_TRIANGLES, 0, 3);

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse
      // moved etc.)
      // -------------------------------------------------------------------------------
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  int initGLFW() {

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return 0;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return 0;
    }
    return 1;
  }

private:
};

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
inline void WindowEngine::processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
inline void WindowEngine::framebuffer_size_callback(GLFWwindow *window,
                                                    int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
