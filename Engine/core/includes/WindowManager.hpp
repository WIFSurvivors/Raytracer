#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

struct WindowManager {
  //  This function is called, whenever the window size changes
  static void framebuffer_size_callback(GLFWwindow *window, int new_width,
                                        int new_height);
  static void processInput(GLFWwindow *window);

  WindowManager();

  bool shouldClose();
  glm::vec2 getMousePos();
  glm::ivec2 getScreenSize() const;
  void draw(); // make function pointer to RenderSystem??
  void updateInput();
  void swapBuffers();

  double get_time();

private:
  GLFWwindow *_window;

  glm::vec2 _mousePos{0., 0.};
  glm::ivec2 _screenSize{800, 800};

  bool _initGLFW();
};
