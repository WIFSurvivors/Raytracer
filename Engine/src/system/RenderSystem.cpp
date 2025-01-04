#include "includes/system/RenderSystem.hpp"
#include "includes/WindowManager.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/ShaderCompiler.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/FrameSnapshot.hpp"
#include "includes/utility/bvhtree_tiny.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <memory>
#include <iostream>
#include <filesystem>
#include <map>

#ifndef SHADER_ABSOLUTE_PATH
#define SHADER_ABSOLUTE_PATH "wawawaww"
#endif

// #define SHOW_UI true

/**
 *	TODO:
 *	- Think about projection clipping space
 *	- Better window handling
 *	- Btter error handling
 *	- Refactor render loop
 *	  - e.g. Call it something else
 *	  - Separate other functionality to the functions
 */

RenderSystem::RenderSystem(UUIDManager *um, WindowManager *wm, CameraSystem *cs,
                           LightSystem *ls)
    : System{um}, _wm{wm}, _cs{cs}, _ls{ls} {
  LOG("created render system");
  init();
}

void RenderSystem::init() {
#if SHOW_UI
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_ERROR("Failed to initialize GLAD");
    return;
  }

  std::filesystem::path shader_folder(SHADER_ABSOLUTE_PATH);
  std::filesystem::path compute_shader_file =
      shader_folder / "computeShaderWithTriangles.glsl";
  std::filesystem::path vertex_shader_file =
      shader_folder / "vertexshader.glsl";
  std::filesystem::path fragment_shader_file =
      shader_folder / "fragmentshader.glsl";

  LOG(std::format("Shader File Path: {}", fragment_shader_file.string()));
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

  _ls_active_light_sourcesU =
      glGetUniformLocation(compute->programID, "ls_active_light_sources");
  _ls_positionsU = glGetUniformLocation(compute->programID, "ls_positions");
  _ls_directionsU = glGetUniformLocation(compute->programID, "ls_directions");
  _ls_colorsU = glGetUniformLocation(compute->programID, "ls_colors");
  _ls_intensitiesU = glGetUniformLocation(compute->programID, "ls_intensities");

  std::vector<Triangle> triforce2 = createCube(glm::vec3{0.0f, -2.0f, 0.0f});
  std::vector<Triangle> triforce1 = createCube(glm::vec3{2.0f, 0.0f, 0.0f});
  std::vector<Triangle> triforce3 = createCube(glm::vec3{-2.0f, 0.0f, 0.0f});

  std::vector<Triangle> triforce = triforce1;
  triforce.insert(triforce.end(), triforce2.begin(), triforce2.end());
  triforce.insert(triforce.end(), triforce3.begin(), triforce3.end());
  LOG(std::format("Triforce size: {}", triforce.size()));

  TreeBuilder builder{};
  builder.prepareSSBOData();
  //   builder.checkData(); // Debug statements

  LOG(std::format("SSBONodes size: {}", sizeof(SSBONodes)));

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
  LOG(std::format("Max work groups per compute shader x:{} y:{} z:{}",
                  work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]));

  int work_grp_size[3];
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
  LOG(std::format("Max work group sizes x:{} y:{} z:{}", work_grp_size[0],
                  work_grp_size[1], work_grp_size[2]));

  int work_grp_inv;
  glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
  LOG(std::format("Max invocations count per work group: {}", work_grp_inv));
#endif
}

void RenderSystem::update(const FrameSnapshot &snapshot) {
#if SHOW_UI
  //  Specifies the background color1
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  //  Calculation for the Camera

  //  Setup compute shader
  compute->activateShader();
  glUniform1f(_timeU, snapshot.get_total_time());

  // === CAMERA ====
  if (_cs && _cs->get_main_camera()) {
    _cameraPosition =
        _cs->get_main_camera()->get_entity()->get_world_position();

    // DELETE THIS LINE AT SOME POINT :C
    // _cs->get_main_camera()->get_entity()->set_local_position(
    //     glm::vec3(0.0f, 8.0f, 15.0f));
  } else {
    LOG_ERROR("No main camera found -> using 0., 0., +10.");
    _cameraPosition = glm::vec3{0., 0., +10.};
  }
  glUniform3fv(_cameraU, 1, &_cameraPosition[0]);

  // === LIGHT ====
  if (_ls) {
    auto size = static_cast<GLint>(_ls->get_components().size());
    glUniform1i(_ls_active_light_sourcesU, size);

    auto positions = _ls->get_positions();
    glUniform3fv(_ls_positionsU, size, &positions[0][0]);

    auto directions = _ls->get_directions();
    glUniform3fv(_ls_directionsU, size, &directions[0][0]);

    auto colors = _ls->get_colors();
    glUniform3fv(_ls_colorsU, size, &colors[0][0]);

    auto intensities = _ls->get_intensities();
    glUniform1fv(_ls_intensitiesU, size, intensities.data());
  }

  glUniformMatrix4fv(_projU, 1, GL_FALSE, &_projectionMatrix[0][0]);
  glUniformMatrix4fv(_viewU, 1, GL_FALSE, &_viewMatrix[0][0]);

  auto screen_size = _wm->get_screen_size();
  // int groupsX = (screen_size.x + 16 - 1) / 16;
  // int groupsY = (screen_size.y + 16 - 1) / 16;
  glDispatchCompute(ceil(screen_size.x / 32.0), ceil(screen_size.y / 32.0), 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  // Setup fragment and vertex shader
  program->activateShader();
  glBindVertexArray(_vao);

  for (auto &&c : _components) {
    c.second->update(snapshot);
  }
#endif
}

RenderComponent *RenderSystem::create_component(Entity *e) {
  LOG("create render component (a1)");
  auto c = create_component_base(e);
  // TODO: add default parameters!
  return c;
}

RenderComponent *RenderSystem::create_component(Entity *e, uuid id) {
  LOG("create render component (a2)");
  auto c = create_component_base(e, id);
  // TODO: add default parameters!
  return c;
}

RenderComponent *
RenderSystem::create_component(Entity *e,
                               const std::vector<glm::vec3> &vertices,
                               const std::vector<glm::vec2> &UV) {
  LOG("create render component (b1)");
  auto c = create_component_base(e);
  c->set_vertices(vertices);
  c->set_uv(UV);

  int programmID = 0;
#if SHOW_UI
  programmID = program->programID;
#endif
  c->init(programmID);
  return c;
}

RenderComponent *
RenderSystem::create_component(Entity *e, uuid id,
                               const std::vector<glm::vec3> &vertices,
                               const std::vector<glm::vec2> &UV) {
  LOG("create render component (b2)");
  auto c = create_component_base(e, id);
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
  std::cout << std::endl;
}
