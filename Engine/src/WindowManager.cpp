#include "includes/WindowManager.hpp"
#include "includes/utility/Log.hpp"
#include <iostream>

#if SHOW_UI
void WindowManager::processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void WindowManager::framebuffer_size_callback(GLFWwindow *window, int width,
                                              int height) {
  // make sure the viewport matches the new window dimensions;
  // note that width and height will be significantly larger than specified on
  // retina displays.
  glad_glViewport(0, 0, width, height);
}
#endif

WindowManager::WindowManager() { _initGLFW(); }

void WindowManager::update_input() {
#if SHOW_UI
  glfwPollEvents();
  processInput(_window);

  glm::dvec2 temp{};
  glfwGetCursorPos(_window, &temp.x, &temp.y);
  _mousePos = static_cast<glm::vec2>(temp);

  // probably move this one, but for now it can stay
  // alternatively rename method name
  glfwGetWindowSize(_window, &_screenSize.x, &_screenSize.y);
#endif
}

void WindowManager::close() {
#if SHOW_UI
  glfwSetWindowShouldClose(_window, GLFW_FALSE);
// glfwDestroyWindow(_window);
#endif
}
void WindowManager::swap_buffers() {
#if SHOW_UI
  glfwSwapBuffers(_window);
#endif
}

bool WindowManager::should_close() {
#if SHOW_UI
  return !glfwWindowShouldClose(_window);
#endif
}

glm::vec2 WindowManager::get_mouse_pos() const { return _mousePos; }

glm::ivec2 WindowManager::get_screen_size() const { return _screenSize; }

bool WindowManager::_initGLFW() {
#if SHOW_UI
  LOG("Init GLFW");
  if (!glfwInit()) {
    LOG_ERROR("Failed to init glfw");
    return false;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  // glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  // std::cout << "Im gonna create a window\n";
  _window = glfwCreateWindow(_screenSize.x, _screenSize.y, "RenderWindow", NULL,
                             NULL);
  if (_window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return 0;
  }
  glfwMakeContextCurrent(_window);
  std::cout << _window << "\n";
  glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
  return true;
#endif
}
