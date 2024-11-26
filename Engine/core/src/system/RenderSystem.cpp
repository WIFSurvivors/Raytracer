#include "includes/system/RenderSystem.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/ShaderCompiler.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include <iostream>

namespace core {
/**
 *	TODO:
 *	- Think about projection clipping space
 *	- Better window handling
 *	- Btter error handling
 *	- Refactor render loop
 *	  - e.g. Call it something else
 *	  - Separate other functionality to the functions
 */
void RenderSystem::init() {
  initGLFW();

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  //  I dont like this at all
  Shader simpleShader{
      std::make_pair(GL_VERTEX_SHADER, "../shaders/vertexshader.glsl"),
      std::make_pair(GL_FRAGMENT_SHADER, "../shaders/fragmentshader.glsl")};
  program = std::make_unique<Shader>(simpleShader);
  //  program->activateShader();
  //  I dont like this at all
  Shader computeShader{
      std::make_pair(GL_COMPUTE_SHADER, "../shaders/computeshaderCircle.glsl")};
  compute = std::make_unique<Shader>(computeShader);

  //  mouseUniformID = glGetUniformLocation(computeShader.programID,
  //  "mousePos");

  //  Jeb, i don't know what to do
  //  And I dont like that I give a programID here
  _component->init(simpleShader.programID);
  //  compute->activateShader();
  mouseUniformID = glGetUniformLocation(computeShader.programID, "mousePos");
  compute->activateShader();
  program->activateShader();
}

void RenderSystem::update() {
  //  Render

  //  Specifies the background color1
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glm::vec2 puk = glm::vec2(_mouseX, _mouseY);

  //  glUniform2fv(mouseUniformID, 1, &puk[0]);

  // Jeb, i dont know what to do
  // I moved it here because i want to have acces to the updated data of Model
  _component->update();
  float radius = 10.0f;
  float camX = sin(glfwGetTime()) * radius;
  float camZ = cos(glfwGetTime()) * radius;
  _view = glm::lookAt(glm::vec3(0.0, 10.0, 10.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));  

  //  Setup compute shader
  compute->activateShader();
      glUniform1f(glGetUniformLocation(compute->programID, "time"),
                  glfwGetTime());

      glUniformMatrix4fv(glGetUniformLocation(compute->programID, "Model"), 1, GL_FALSE, glm::value_ptr(*_component->getModelMatrice()));
      glUniformMatrix4fv(glGetUniformLocation(compute->programID, "View"), 1, GL_FALSE, glm::value_ptr(_view));
      glUniformMatrix4fv(glGetUniformLocation(compute->programID, "Projection"), 1, GL_FALSE, glm::value_ptr(_projectionMatrix));
      glUniform2fv(glGetUniformLocation(compute->programID, "imageSize"),
                   1, glm::value_ptr(glm::vec2(_scr_width, _scr_height)));
    glUniform2fv(mouseUniformID, 1, &puk[0]);
  //  Self explanatory
  //  Dispateches the compute shader with SCR_WIDTH*SCR_HEIGHT*1 = number of work
  //  groups
  glDispatchCompute(_scr_width, _scr_height, 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  // Setup fragment and vertex shader
  program->activateShader();
  //draw
  _component->draw(_vao);
}

void RenderSystem::destroy() {
  _component->destroy();

  glDeleteVertexArrays(1, &_vao);

  glfwTerminate();
}

void RenderSystem::render() {
  while (!glfwWindowShouldClose(_window)) {
    glfwGetWindowSize(_window, &_scr_width, &_scr_height);
    // Input
    processInput(_window);

    glfwGetCursorPos(_window, &_mouseX, &_mouseY);
    std::cout << "X: " << static_cast<int>(_mouseX) << ", Y: " << static_cast<int>(_mouseY) << "\n";
    update();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse
    // moved etc.)
    glfwSwapBuffers(_window);
    glfwPollEvents();
  }
}

int RenderSystem::initGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
 // glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  _window =
      glfwCreateWindow(_scr_width, _scr_height, "RenderWindow", NULL, NULL);
  if (_window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return 0;
  }
  glfwMakeContextCurrent(_window);
  glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return 0;
  }
  return 1;
}

void RenderSystem::processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void RenderSystem::framebuffer_size_callback(GLFWwindow *window, int width,
                                             int height) {
  // make sure the viewport matches the new window dimensions;
  // note that width and height will be significantly larger than specified on
  // retina displays.
  glViewport(0, 0, width, height);
}

} // namespace core