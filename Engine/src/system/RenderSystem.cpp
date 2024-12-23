#include "includes/system/RenderSystem.hpp"
#include "includes/WindowManager.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/ShaderCompiler.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/bvhtree_tiny.hpp"

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
  Log::message("created render system");
  init();
}

void RenderSystem::init() {
#if SHOW_UI
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    Log::error("Failed to initialize GLAD");
    return;
  }

  std::filesystem::path shader_folder(SHADER_ABSOLUTE_PATH);
  std::filesystem::path compute_shader_file =
      shader_folder / "computeShaderWithTriangles.glsl";
  std::filesystem::path vertex_shader_file =
      shader_folder / "vertexshader.glsl";
  std::filesystem::path fragment_shader_file =
      shader_folder / "fragmentshader.glsl";

  Log::message(std::format("Shader File Path: {}", fragment_shader_file.string()));
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  Shader simpleShader{
      std::make_pair(GL_VERTEX_SHADER, vertex_shader_file.string()),
      std::make_pair(GL_FRAGMENT_SHADER, fragment_shader_file.string())};
  program = std::make_unique<Shader>(simpleShader);

  Shader computeShader{
      std::make_pair(GL_COMPUTE_SHADER, compute_shader_file.string())};
  compute = std::make_unique<Shader>(computeShader);

  _cameraPosition = glm::vec3(0.0f, 8.0f, 15.0f);
  _cameraDirection = glm::vec3(0.0f, 3.0f, 0.0f);
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

  std::vector<Triangle> triforce2 = createCube(glm::vec3{0.0f, -2.0f, 0.0f});
  std::vector<Triangle> triforce1 = createCube(glm::vec3{2.0f, 0.0f, 0.0f});
  std::vector<Triangle> triforce3 = createCube(glm::vec3{-2.0f, 0.0f, 0.0f});

  std::vector<Triangle> triforce = triforce1;
  triforce.insert(triforce.end(), triforce2.begin(), triforce2.end());
  triforce.insert(triforce.end(), triforce3.begin(), triforce3.end());
  Log::message(std::format("Triforce size: {}", triforce.size()));

  TreeBuilder builder{};
  builder.prepareSSBOData();
  builder.checkData();

  Log::message(std::format("SSBONodes size: {}", sizeof(SSBONodes)));

  glGenBuffers(1, &ssbo_tree);
  glGenBuffers(1, &ssbo_indices);
  glGenBuffers(1, &ssbo_vertex);
  glGenBuffers(1, &ssbo_mats);
  glGenBuffers(1, &ssbo_matsIDX);

  glGenBuffers(1, &ssbo_triangle);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_tree);

  glBufferData(GL_SHADER_STORAGE_BUFFER,
               builder.ssboData.size() * sizeof(SSBONodes),
               builder.ssboData.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo_tree);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_indices);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               builder.triIdxData.size() * sizeof(uint32_t),
               builder.triIdxData.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo_indices);

  // Depracated lol
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_triangle);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               builder.triangles.size() * sizeof(Triangle),
               builder.triangles.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_triangle);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vertex);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               builder.vertex.size() * sizeof(Vec3Padded),
               builder.vertex.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo_vertex);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_mats);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               builder.mats.size() * sizeof(Materials), builder.mats.data(),
               GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo_mats);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_matsIDX);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               builder.matIndx.size() * sizeof(uint32_t),
               builder.matIndx.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbo_matsIDX);

  // DEBUG INFORMATION
  //

  int work_grp_cnt[3];
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
  Log::message(std::format("Max work groups per compute shader x:{} y:{} z:{}",
                           work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]));

  int work_grp_size[3];
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
  Log::message(std::format("Max work group sizes x:{} y:{} z:{}",
                           work_grp_size[0], work_grp_size[1],
                           work_grp_size[2]));

  int work_grp_inv;
  glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
  Log::message(
      std::format("Max invocations count per work group: {}", work_grp_inv));
#endif
}

void RenderSystem::update(const float dt) {
#if SHOW_UI
  static auto lastTime = std::chrono::high_resolution_clock::now();
  static int frameCount = 0;
  static double elapsedTime = 0.0;
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
    _cs->get_main_camera()->get_entity()->set_local_position(
        glm::vec3(0.0f, 8.0f, 15.0f));
  } else {
    Log::error("-- ERROR: No main camera found -> using 0., 0., +10.");
    _cameraPosition = glm::vec3{0., 0., +10.};
  }
  glUniform3fv(_cameraU, 1, &_cameraPosition[0]);

  glUniformMatrix4fv(_projU, 1, GL_FALSE, &_projectionMatrix[0][0]);
  glUniformMatrix4fv(_viewU, 1, GL_FALSE, &_viewMatrix[0][0]);

  auto screen_size = _wm->getScreenSize();
  // int groupsX = (screen_size.x + 16 - 1) / 16;
  // int groupsY = (screen_size.y + 16 - 1) / 16;
  glDispatchCompute(ceil(screen_size.x / 32.0), ceil(screen_size.y / 32.0), 1);
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
  //

  frameCount++;
  auto currentTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> frameTime = currentTime - lastTime;
  lastTime = currentTime;
  elapsedTime += frameTime.count();
  // Print FPS every 1 second
  if (elapsedTime >= 1.0) {
    double fps = frameCount / elapsedTime;
    Log::message(std::format("FPS: {}", fps));

    frameCount = 0;
    elapsedTime = 0.0;
  }
#endif
}

RenderComponent *RenderSystem::create_component(uuid id, Entity *e) {
  Log::message("create render component (a)");
  auto c = create_component_base(id, e);
  // TODO: add default parameters!
  return c;
}

RenderComponent *
RenderSystem::create_component(uuid id, Entity *e,
                               const std::vector<glm::vec3> &vertices,
                               const std::vector<glm::vec2> &UV) {
  Log::message("create render component (b)");
  auto c = create_component_base(id, e);
  c->set_vertices(vertices);
  c->set_uv(UV);

  int programmID = 0;
#if SHOW_UI
  programmID = program->programID;
#endif
  c->init(programmID);
  return c;
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
  Log::new_line();
}
