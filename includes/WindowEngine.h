#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>




struct WindowEngine{

  void static framebuffer_size_callback(GLFWwindow* window, int width, int height);
  void static processInput(GLFWwindow *window);

  uint32_t SCR_HEIGHT = 800;
  uint32_t SCR_WIDTH = 800;

  GLFWwindow* window;

  void initializeEngine(){
    if (!initGLFW()) throw std::runtime_error{"Failed to Initialize Engine"};
    render_loop();
    terminate();
  }

  void terminate(){
    //Do Deletion Here
    glfwTerminate();
  }

  void render_loop(){
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

  }

  int initGLFW(){
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }    
    return 1;
  }



  private:




};

  // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
  // ---------------------------------------------------------------------------------------------------------
  inline void WindowEngine::processInput(GLFWwindow *window)
  {
      if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
          glfwSetWindowShouldClose(window, true);
  }

  // glfw: whenever the window size changed (by OS or user resize) this callback function executes
  // ---------------------------------------------------------------------------------------------
  inline void WindowEngine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
      // make sure the viewport matches the new window dimensions; note that width and 
      // height will be significantly larger than specified on retina displays.
      glViewport(0, 0, width, height);
  }
