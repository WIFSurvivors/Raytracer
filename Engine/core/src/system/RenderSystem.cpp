#include "includes/system/RenderSystem.hpp"
#include "includes/WindowManager.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/ShaderCompiler.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include "includes/utility/TablePrinter.hpp"
#include "includes/utility/SimpleLogger.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <memory>

#include <iostream>
#include <filesystem>
/**
 *	TODO:
 *	- Think about projection clipping space
 *	- Better window handling
 *	- Btter error handling
 *	- Refactor render loop
 *	  - e.g. Call it something else
 *	  - Separate other functionality to the functions
 */

RenderSystem::RenderSystem(WindowManager *wm, CameraSystem* cs) : System(), _wm{wm}, _cs{cs} {
  SimpleLogger::print("-- created entity system");
}

void RenderSystem::init() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }
  
  std::filesystem::path shader_folder(SHADER_ABSOLUTE_PATH);
  std::filesystem::path compute_shader_file =
      shader_folder / "computeshaderCircle.glsl";
  std::filesystem::path vertex_shader_file =
      shader_folder / "vertexshader.glsl";
  std::filesystem::path fragment_shader_file =
      shader_folder / "fragmentshader.glsl";

  std::cout << "FILE PATH: " << fragment_shader_file.string() << std::endl;
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  Shader simpleShader{
      std::make_pair(GL_VERTEX_SHADER, vertex_shader_file.string()),
      std::make_pair(GL_FRAGMENT_SHADER, fragment_shader_file.string())};
  program = std::make_unique<Shader>(simpleShader);

  Shader computeShader{
      std::make_pair(GL_COMPUTE_SHADER, compute_shader_file.string())};
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
  if(_cs && _cs->get_main_camera()){
  	_cameraPosition = _cs->get_main_camera()->get_entity()->get_world_position();
  }else {
	SimpleLogger::print("-- ERROR: No main camera found -> using 0., 0., +10.");
	_cameraPosition = glm::vec3{0., 0., +10.};
  }
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
    c.second->update(dt);
  }
  // Input
  // processInput(_window);

  // update(glfwGetTime());
}

Component *RenderSystem::create_component(uuid id, Entity *e) {
  throw NotImplementedError();
  /*
  _components[id] =
      std::make_unique<RenderComponent>(id, e, program->programID);
  auto ptr = _components[id].get();
  // ptr->init(program->programID);
  e->add_component(ptr);
  return ptr;
  */
}

RenderComponent *
RenderSystem::create_component(uuid id, Entity *e,
                               const std::vector<glm::vec3> &vertices,
                               const std::vector<glm::vec2> &UV) {
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

bool RenderSystem::remove(Component *c) { throw NotImplementedError(); }
bool RenderSystem::remove(uuid uuid) { throw NotImplementedError(); }

void RenderSystem::print() {
  TablePrinter::printElement("RenderComponent UUID", 36);
  std::cout << " | ";
  TablePrinter::printElement("VBO", 12);
  std::cout << " | ";
  TablePrinter::printElement("textureID", 12);
  std::cout << " | ";
  TablePrinter::printElement("uvVBO", 12);
  std::cout << "\n";
  std::cout << std::string(36 + 12 + 12 + 12 + 3 * 3, '=');
  std::cout << "\n";
  for (auto const &[uuid, c] : _components) {
    std::cout << uuid << " | ";
    if (c == nullptr) {
      std::cout << "missing...\n";
      continue;
    }
    TablePrinter::printElement(c->get_vbo(), 12);
    std::cout << " | ";
    TablePrinter::printElement(c->get_textureID(), 12);
    std::cout << " | ";
    TablePrinter::printElement(c->get_uvVBO(), 12);
    std::cout << "\n";
  }
  std::cout << std::endl;
}
