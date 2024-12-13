#include "includes/WindowManager.hpp"
#include <iostream>

#ifdef SHOW_UI
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

WindowManager::WindowManager() { _initGLFW(); }

void WindowManager::updateInput() {
  glfwPollEvents();
  processInput(_window);

  glm::dvec2 temp{};
  glfwGetCursorPos(_window, &temp.x, &temp.y);
  _mousePos = static_cast<glm::vec2>(temp);

  // probably move this one, but for now it can stay
  // alternatively rename method name
  glfwGetWindowSize(_window, &_screenSize.x, &_screenSize.y);
}

void WindowManager::swapBuffers() { glfwSwapBuffers(_window); }

bool WindowManager::shouldClose() { return !glfwWindowShouldClose(_window); }
void WindowManager::close() {
  glfwSetWindowShouldClose(_window, GLFW_FALSE);
  // glfwDestroyWindow(_window);
}

glm::vec2 WindowManager::getMousePos() { return _mousePos; }

glm::ivec2 WindowManager::getScreenSize() const { return _screenSize; }

double WindowManager::get_time() { return glfwGetTime(); }

bool WindowManager::_initGLFW() {
  if (!glfwInit()) {
    std::cout << "Failed to init glfw\n";
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
}

void WindowManager::update() {
  updateInput();
  glfwSwapBuffers(_window);
}

#else

WindowManager::WindowManager() { _initGLFW(); }

void WindowManager::updateInput() {}

void WindowManager::swapBuffers() { }

bool WindowManager::shouldClose() { return false; }
void WindowManager::close() { }

glm::vec2 WindowManager::getMousePos() { return _mousePos; }

glm::ivec2 WindowManager::getScreenSize() const { return _screenSize; }

double WindowManager::get_time() { return 0.; }

bool WindowManager::_initGLFW() { return false; }

void WindowManager::update() {}
#endif