#include "includes/system/RenderSystem.hpp"
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
  std::cout << "rs: a\n";
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }
  std::cout << "rs: b\n";

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  std::cout << "rs: c\n";
  //  I dont like this at all
  Shader simpleShader{
      std::make_pair(GL_VERTEX_SHADER, "../core/shaders/vertexshader.glsl"),
      std::make_pair(GL_FRAGMENT_SHADER, "../core/shaders/fragmentshader.glsl")};
  program = std::make_unique<Shader>(simpleShader);
  //  program->activateShader();
  //  I dont like this at all
  Shader computeShader{
      std::make_pair(GL_COMPUTE_SHADER, "../core/shaders/computeshader.glsl")};
  compute = std::make_unique<Shader>(computeShader);

  std::cout << "rs: d\n";
  //  mouseUniformID = glGetUniformLocation(computeShader.programID,
  //  "mousePos");

  //  Jeb, i don't know what to do
  //  And I dont like that I give a programID here
  // THIS IS WRONG
  // _component->init(simpleShader.programID);
  std::cout << "rs: e\n";
  //  compute->activateShader();
  // mouseUniformID = glGetUniformLocation(computeShader.programID, "mousePos");
  std::cout << "rs: f\n";
  // compute->activateShader();
  std::cout << "rs: g\n";
  // program->activateShader();
  std::cout << "rs: h\n";
}

void RenderSystem::update(const float dt) {
  //  Specifies the background color1
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  //  Setup compute shader
  compute->activateShader();
  glUniform1f(glGetUniformLocation(compute->programID, "time"), dt);
  glUniform3fv(glGetUniformLocation(compute->programID, "v0"), 1,
               glm::value_ptr(v[0]));
  glUniform3fv(glGetUniformLocation(compute->programID, "v1"), 1,
               glm::value_ptr(v[1]));
  glUniform3fv(glGetUniformLocation(compute->programID, "v2"), 1,
               glm::value_ptr(v[2]));
  glUniform2fv(glGetUniformLocation(compute->programID, "imageSize"), 1,
               glm::value_ptr((glm::vec2)_wm->getScreenSize()));
  glUniform2fv(mouseUniformID, 1, &_wm->getMousePos()[0]);
  //  Self explanatory
  //  Dispateches the compute shader with SCR_WIDTH*SCR_HEIGHT*1 = number of
  //  work groups
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

RenderComponent *RenderSystem::create_component(uuid id, Entity *e, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& UV) {
  _components[id] = std::make_unique<RenderComponent>(id, e, program->programID, vertices, UV);
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
