#include "includes/RenderSystem.hpp"
#include "includes/RenderComponent.hpp"
#include "includes/ShaderCompiler.hpp"

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
      std::make_pair(GL_VERTEX_SHADER, "../shaders/vertexshader.glsl"),
      std::make_pair(GL_FRAGMENT_SHADER, "../shaders/fragmentshader.glsl")};
  program = std::make_unique<Shader>(simpleShader);
  //  program->activateShader();
  //  I dont like this at all
  Shader computeShader{
      std::make_pair(GL_COMPUTE_SHADER, "../shaders/computeshaderCircle.glsl")};
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
  compute->activateShader();
  std::cout << "rs: g\n";
  program->activateShader();
  std::cout << "rs: h\n";
}

void RenderSystem::update(const float dt) {
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
  //  Dispateches the compute shader with SCR_WIDTH*SCR_HEIGHT*1 = number of
  //  work groups
  auto screen_size = _wm->getScreenSize();
  glDispatchCompute(screen_size.x, screen_size.y, 1);
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

void RenderSystem::render() {}

} // namespace core
