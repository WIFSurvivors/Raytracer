#pragma once
#include "ShaderCompiler.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

struct WindowEngine {
  void static framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);
  void static processInput(GLFWwindow *window);

  uint32_t SCR_HEIGHT = 800;
  uint32_t SCR_WIDTH = 800;
  float deltaTime = 0.0f; // time between current frame and last frame
  float lastFrame = 0.0f; // time of last frame

  GLFWwindow *window;
  GLuint VBO;

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

  void render_loop() {

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                        0.0f,  0.0f,  0.5f, 0.0f};

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    Shader simpleShader{
        std::make_pair(GL_VERTEX_SHADER, "../shaders/vertexshader.glsl"),
        std::make_pair(GL_FRAGMENT_SHADER, "../shaders/fragmentshader.glsl")};

    // Query

    int max_compute_work_group_count[3];
    int max_compute_work_group_size[3];
    int max_compute_work_group_invocations;

    for (int idx = 0; idx < 3; idx++) {
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, idx,
                      &max_compute_work_group_count[idx]);
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, idx,
                      &max_compute_work_group_size[idx]);
    }
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS,
                  &max_compute_work_group_invocations);

    std::cout << "OpenGL Limitations: " << std::endl;
    std::cout << "maximum number of work groups in X dimension "
              << max_compute_work_group_count[0] << std::endl;
    std::cout << "maximum number of work groups in Y dimension "
              << max_compute_work_group_count[1] << std::endl;
    std::cout << "maximum number of work groups in Z dimension "
              << max_compute_work_group_count[2] << std::endl;

    std::cout << "maximum size of a work group in X dimension "
              << max_compute_work_group_size[0] << std::endl;
    std::cout << "maximum size of a work group in Y dimension "
              << max_compute_work_group_size[1] << std::endl;
    std::cout << "maximum size of a work group in Z dimension "
              << max_compute_work_group_size[2] << std::endl;

    std::cout << "Number of invocations in a single local work group that may "
                 "be dispatched to a compute shader "
              << max_compute_work_group_invocations << std::endl;

    Shader computeShader{
        std::make_pair(GL_COMPUTE_SHADER, "../shaders/computeshader.glsl")};
    simpleShader.activateShader();
    glUniform1i(glGetUniformLocation(simpleShader.programID, "tex"), 0);

    unsigned int texture;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1000, 1000, 0, GL_RGBA, GL_FLOAT,
                 NULL);

    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    std::cout << "here\n";
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    int fCounter = 0;
    while (!glfwWindowShouldClose(window)) {
      // Set frame time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		if(fCounter > 500) {
			std::cout << "FPS: " << 1 / deltaTime << std::endl;
			fCounter = 0;
		} else {
			fCounter++;
		}
      // input
      // -----
      processInput(window);

      computeShader.activateShader();
      glUniform1f(glGetUniformLocation(computeShader.programID, "t"), currentFrame);
      glDispatchCompute((unsigned int)100, (unsigned int)100, 1);
      glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

      // render
      // ------
      // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
