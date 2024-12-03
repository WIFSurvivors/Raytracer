#include "includes/system/RenderSystem.hpp"
#include "includes/WindowManager.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/ShaderCompiler.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include <iostream>

/**
 *	TODO:
 *	- Think about projection clipping space
 *	- Better window handling
 *	- Btter error handling
 *	- Refactor render loop
 *	  - e.g. Call it something else
 *	  - Separate other functionality to the functions
 */

RenderSystem::RenderSystem(WindowManager *wm) : _wm{wm} {}

void RenderSystem::init() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  Shader simpleShader{
      std::make_pair(GL_VERTEX_SHADER, "../core/shaders/vertexshader.glsl"),
      std::make_pair(GL_FRAGMENT_SHADER,
                     "../core/shaders/fragmentshader.glsl")};
  program = std::make_unique<Shader>(simpleShader);

  Shader computeShader{std::make_pair(
      GL_COMPUTE_SHADER, "../core/shaders/computeshaderCircle.glsl")};
  compute = std::make_unique<Shader>(computeShader);

  _cameraPosition = glm::vec3(0.0f, 10.0f, 10.0f);
  _cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
  _viewMatrix =
      glm::lookAt(_cameraPosition, _cameraDirection, glm::vec3(0, 1, 0));
  //  TODO:
  //  calculate the aspect ratio appropriately
  _fov = 60.0f;
  _projectionMatrix = glm::perspective(glm::radians(_fov), 1.0f, 0.1f, 100.0f);

  _timeU = glGetUniformLocation(compute->programID, "time");
  _cameraU = glGetUniformLocation(compute->programID, "cameraPos");
  _projU = glGetUniformLocation(compute->programID, "Projection");
  _viewU = glGetUniformLocation(compute->programID, "View");
}

void RenderSystem::update(const float dt) {
  //  Specifies the background color1
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  //  Calculation for the Camera

  //  Setup compute shader
  compute->activateShader();
  glUniform1f(_timeU, dt);
  glUniform3fv(_cameraU, 1, &_cameraPosition[0]);
  glUniformMatrix4fv(_projU, 1, GL_FALSE, &_projectionMatrix[0][0]);
  glUniformMatrix4fv(_viewU, 1, GL_FALSE, &_viewMatrix[0][0]);

  auto screen_size = _wm->getScreenSize();
  glDispatchCompute(screen_size.x, screen_size.y, 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  // Setup fragment and vertex shader
  program->activateShader();
  glBindVertexArray(_vao);
  // _component->update();
  for (auto &&c : _components) {
    c.second->update();
  }
  // Input
  // processInput(_window);

  // update(glfwGetTime());
}

Component *RenderSystem::create_component(uuid id, Entity *e) {
  _components[id] =
      std::make_unique<RenderComponent>(id, e, program->programID);
  auto ptr = _components[id].get();
  // ptr->init(program->programID);
  e->add_component(ptr);
  return ptr;
}

RenderComponent *
RenderSystem::create_component(uuid id, Entity *e,
                               std::vector<glm::vec3> &vertices,
                               std::vector<glm::vec2> &UV) {
  _components[id] = std::make_unique<RenderComponent>(id, e, program->programID,
                                                      vertices, UV);
  auto ptr = _components[id].get();
  // ptr->init(program->programID);
  e->add_component(ptr);
  return ptr;
}

void RenderSystem::destroy() {
  // _component->destroy();

  glDeleteVertexArrays(1, &_vao);

  glfwTerminate();
}

// void RenderSystem::render() {
// glfwSetInputMode(_wm->_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

// glfwMakeContextCurrent(_wm->_window);

// }