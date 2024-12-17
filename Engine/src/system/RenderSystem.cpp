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

#ifndef SHADER_ABSOLUTE_PATH
#define SHADER_ABSOLUTE_PATH "wawawaww"
#endif

/**
 *	TODO:
 *	- Think about projection clipping space
 *	- Better window handling
 *	- Btter error handling
 *	- Refactor render loop
 *	  - e.g. Call it something else
 *	  - Separate other functionality to the functions
 */

RenderSystem::RenderSystem(WindowManager *wm, CameraSystem *cs)
    : System(), _wm{wm}, _cs{cs} {
  SimpleLogger::print("-- created render system");
  // init(); // ? just do here ?
}

void RenderSystem::init() {
#if SHOW_UI
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
#endif
}

void RenderSystem::update(const float dt) {
#if SHOW_UI
  //  Specifies the background color1
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  //  Calculation for the Camera

  //  Setup compute shader
  compute->activateShader();
  glUniform1f(_timeU, dt);

  if (_cs && _cs->get_main_camera()) {
    _cameraPosition =
        _cs->get_main_camera()->get_entity()->get_world_position();
  } else {
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
#endif
}

RenderComponent *
RenderSystem::create_component(uuid id, Entity *e,
                               const std::vector<glm::vec3> &vertices,
                               const std::vector<glm::vec2> &UV) {
  SimpleLogger::print("-- create render component");
  //   auto c = create_component_base(id, e); // ADD THIS

  // CHANGE RC CONSTRUCTR :C
  int programmID = 0;
#if SHOW_UI
  programmID = program->programID;
#endif
  _components[id] =
      std::make_unique<RenderComponent>(id, e, programmID, vertices, UV);
  auto ptr = _components[id].get();
  // ptr->init(program->programID);
  e->add_component(ptr);
  return ptr;
}

void RenderSystem::destroy() {
#if SHOW_UI
  // _component->destroy();

  glDeleteVertexArrays(1, &_vao);

  glfwTerminate();
#endif
}

void RenderSystem::print() {
  VariadicTable<std::string, GLint, GLint, GLint, std::string> vt(
      {"RenderComponent UUID", "VBO", "TextureID", "UV VBO", "Entity Name"});

  for (const auto &[key, value] : _components) {
    vt.addRow(boost::uuids::to_string(key), value->get_vbo(),
              value->get_textureID(), value->get_uvVBO(),
              value->get_entity()->get_name());
  }

  vt.print(std::cout);
  std::cout << std::endl;
}