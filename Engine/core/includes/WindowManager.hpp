#pragma once

#include "glad/glad.h"
#ifdef SHOW_UI
#include <GLFW/glfw3.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

#ifdef SHOW_UI
struct WindowManager {
  //  This function is called, whenever the window size changes
  static void framebuffer_size_callback(GLFWwindow *window, int new_width,
                                        int new_height);
  static void processInput(GLFWwindow *window);

  WindowManager();

  void close();
  bool shouldClose();

  glm::vec2 getMousePos();
  glm::ivec2 getScreenSize() const;
  void updateInput();
  void swapBuffers();

  double get_time();
  void update();

private:
  glm::vec2 _mousePos{0., 0.};
  glm::ivec2 _screenSize{800, 800};
  GLFWwindow *_window;

  bool _initGLFW();
};
#else
struct WindowManager {
  WindowManager();

  void close();
  bool shouldClose();

  glm::vec2 getMousePos();
  glm::ivec2 getScreenSize() const;
  void updateInput();
  void swapBuffers();

  double get_time();
  void update();

private:
  glm::vec2 _mousePos{0., 0.};
  glm::ivec2 _screenSize{800, 800};

  bool _initGLFW();
};
#endif