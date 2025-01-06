#pragma once

#if SHOW_UI
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

struct WindowManager {
#if SHOW_UI
  //  This function is called, whenever the window size changes
  static void framebuffer_size_callback(GLFWwindow *window, int new_width,
                                        int new_height);
  static void processInput(GLFWwindow *window);
#endif

  WindowManager();

  void close();
  bool should_close();

  glm::vec2 get_mouse_pos() const;
  glm::ivec2 get_screen_size() const;
  void update_input();
  void swap_buffers();

private:
  inline static glm::vec2 _mousePos{0., 0.};
  inline static glm::ivec2 _screenSize{800, 800};
#if SHOW_UI
  GLFWwindow *_window;
#endif

  bool _initGLFW();
};
