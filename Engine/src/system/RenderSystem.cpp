#include "includes/system/RenderSystem.hpp"
#include "includes/WindowManager.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/ShaderCompiler.hpp"
// #include "includes/utility/NotImplementedError.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/FrameSnapshot.hpp"
#include "includes/AssetManager.hpp"
#include "includes/utility/bvhtree_tiny.hpp"
#include "includes/utility/Canvas.hpp"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <memory>
#include <iostream>
#include <filesystem>
#include <map>

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
// NEED TO FIX
std::unique_ptr<TreeBuilder> BVH_Tree;

namespace RT {
RenderSystem::RenderSystem(UUIDManager *um, WindowManager *wm, CameraSystem *cs,
                           LightSystem *ls, AssetManager::DefaultAssets *da)
    : System{um}, _wm{wm}, _cs{cs}, _ls{ls}, _da{da} {
  LOG("created render system");
  init();
}

void RenderSystem::init() {
#if SHOW_UI
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_ERROR("Failed to initialize GLAD");
    return;
  }

  std::filesystem::path shader_folder(get_relative_shader_folder_path());
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
  _program = std::make_unique<Shader>(simpleShader);

  _canvas =
      std::make_unique<Canvas>(_program->programID, _wm->get_screen_size());

  Shader computeShader{
      std::make_pair(GL_COMPUTE_SHADER, compute_shader_file.string())};
  _compute = std::make_unique<Shader>(computeShader);

  /**************************************************************************/

  _screen_size = _wm->get_screen_size();

  _cameraPosition = glm::vec3(0.0f, 8.0f, 15.0f);
  _cameraDirection = glm::vec3(0.0f, 3.0f, 0.0f);
  _viewMatrix =
      glm::lookAt(_cameraPosition, _cameraDirection, glm::vec3(0, 1, 0));
  //  TODO:
  //  calculate the aspect ratio appropriately

  _projectionMatrix = glm::perspective(glm::radians(_fov), 1.0f, 0.1f, 100.0f);

  _timeU = glGetUniformLocation(_compute->programID, "time");
  _cameraU = glGetUniformLocation(_compute->programID, "cameraPos");
  _projU = glGetUniformLocation(_compute->programID, "Projection");
  _viewU = glGetUniformLocation(_compute->programID, "View");

  // This Uniforoms can be user defined and are required to be defined to work
  // properly
  _maximalBouncesU = glGetUniformLocation(_compute->programID, "bounce");
  _maxHittableTrianglesU =
      glGetUniformLocation(_compute->programID, "hittable");

  _ls_active_light_sourcesU =
      glGetUniformLocation(_compute->programID, "ls_active_light_sources");
  _ls_positionsU = glGetUniformLocation(_compute->programID, "ls_positions");
  _ls_directionsU = glGetUniformLocation(_compute->programID, "ls_directions");
  _ls_colorsU = glGetUniformLocation(_compute->programID, "ls_colors");
  _ls_intensitiesU =
      glGetUniformLocation(_compute->programID, "ls_intensities");

  /**************************************************************************/
  std::vector<Triangle> triforce1 = createCube(glm::vec3{0.0f, -2.0f, 0.0f});
  std::vector<Triangle> triforce2 = createCube(glm::vec3{2.0f, 0.0f, 0.0f});
  std::vector<Triangle> triforce3 = createCube(glm::vec3{-2.0f, 0.0f, 0.0f});
  std::vector<Triangle> triforce4 = createCube(glm::vec3{0.0f, 0.0f, -2.0f});
  std::vector<Triangle> triforce5 = createCube(glm::vec3{0.0f, 2.0f, 0.0f});
  std::vector<Materials> mats;

  Materials Material1 = Materials{
      glm::vec3(0.8f, 0.2f, 0.8f),
      0.0f,
  }; // Light gray, slightly reflective
  Materials Material2 = Materials{glm::vec3(1.0f, 1.0f, 1.0f), 0.0f}; // White
  Materials Material3 = Materials{
      glm::vec3(0.8f, 0.2f, 0.2f),
      0.0f,
  }; // Bright red, more reflective
  Materials Material4 = Materials{
      glm::vec3(0.1f, 0.6f, 0.5f),
      0.0f,
  }; // Bright red, more reflective
  Materials Material5 = Materials{
      glm::vec3(0.0f, 0.6f, 0.9f),
      0.0f,
  }; // Bright red, more reflective
  /**************************************************************************/

  std::vector<ObjectData> data;
  data.push_back(ObjectData(triforce1, Material1));
  data.push_back(ObjectData(triforce2, Material2));
  data.push_back(ObjectData(triforce3, Material3));
  data.push_back(ObjectData(triforce4, Material4));
  data.push_back(ObjectData(triforce5, Material5));
  BVH_Tree = std::make_unique<TreeBuilder>();
  BVH_Tree->loadData(data);
  BVH_Tree->prepareSSBOData();
  //   builder.checkData(); // Debug statements

  LOG(std::format("SSBONodes size: {}", sizeof(SSBONodes)));

  glGenBuffers(1, &_ssbo_tree);
  glGenBuffers(1, &_ssbo_indices);
  glGenBuffers(1, &_ssbo_vertex);
  glGenBuffers(1, &_ssbo_mats);
  glGenBuffers(1, &_ssbo_matsIDX);
  /**/
  /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_tree);*/
  /**/
  /*glBufferData(GL_SHADER_STORAGE_BUFFER,*/
  /*             BVH_Tree->ssboData.size() * sizeof(SSBONodes),*/
  /*             BVH_Tree->ssboData.data(), GL_STATIC_DRAW);*/
  /*glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo_tree);*/
  /**/
  /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_indices);*/
  /*glBufferData(GL_SHADER_STORAGE_BUFFER,*/
  /*             BVH_Tree->triIdxData.size() * sizeof(uint32_t),*/
  /*             BVH_Tree->triIdxData.data(), GL_STATIC_DRAW);*/
  /*glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo_indices);*/
  /**/
  /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vertex);*/
  /*glBufferData(GL_SHADER_STORAGE_BUFFER,*/
  /*             BVH_Tree->vertex.size() * sizeof(Vec3Padded),*/
  /*             BVH_Tree->vertex.data(), GL_STATIC_DRAW);*/
  /*glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo_vertex);*/
  /**/
  /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_mats);*/
  /*glBufferData(GL_SHADER_STORAGE_BUFFER,*/
  /*             BVH_Tree->mats.size() * sizeof(Materials),
   * BVH_Tree->mats.data(),*/
  /*             GL_STATIC_DRAW);*/
  /*glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo_mats);*/
  /**/
  /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_matsIDX);*/
  /*glBufferData(GL_SHADER_STORAGE_BUFFER,*/
  /*             BVH_Tree->matIndx.size() * sizeof(uint32_t),*/
  /*             BVH_Tree->matIndx.data(), GL_STATIC_DRAW);*/
  /*glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbo_matsIDX);*/
  updateSSBOBuffers();
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
  static bool loadData = false;
  std::vector<std::shared_ptr<MeshGallary>> temp_holder;
  for (auto &&c : _components) {
    if (c.second->get_entity()->has_Updated()) {
      glm::vec3 scaleVector = c.second->get_entity()->get_local_scale();
      glm::vec3 translationVector =
          c.second->get_entity()->get_local_position();
      glm::vec3 rotationVector = c.second->get_entity()->get_local_rotation();
      // rotationVector = glm::vec3(0.0f,0.0f,0.0f);

      glm::mat4 ScaleMatrix = glm::scale(glm::mat4{1.0f}, scaleVector);

      glm::mat4 rotationMatrixX =
          glm::rotate(glm::mat4(1.0f), glm::radians(rotationVector.x),
                      glm::vec3(1.0f, 0.0f, 0.0f));
      glm::mat4 rotationMatrixY =
          glm::rotate(glm::mat4(1.0f), glm::radians(rotationVector.y),
                      glm::vec3(0.0f, 1.0f, 0.0f));
      glm::mat4 rotationMatrixZ =
          glm::rotate(glm::mat4(1.0f), glm::radians(rotationVector.z),
                      glm::vec3(0.0f, 0.0f, 1.0f));
      glm::mat4 rotationMatrix =
          rotationMatrixZ * rotationMatrixY * rotationMatrixX;

      glm::mat4 translationMatrix =
          glm::translate(glm::mat4(1.0f), translationVector);
      c.second->set_scaleMatrix(ScaleMatrix);
      c.second->set_rotationMatrix(rotationMatrix);
      c.second->set_translationMatrix(translationMatrix);
      c.second->update_ModelMatrix();

      std::shared_ptr<MeshGallary> object = std::make_unique<MeshGallary>(
          c.second->get_meshes(), c.second->get_entity()->get_uuid(),
          c.second->get_ModelMatrix());
      temp_holder.push_back(object);
      // BVH_Tree->update_gallary(object);
      c.second->get_entity()->did_update();
      std::cout << "UPDATED\n";
      loadData = true;
    }
  }

  if (loadData) {
    BVH_Tree = std::make_unique<TreeBuilder>();
    for (auto &object : temp_holder) {
      update_galary(object);
    }
    BVH_Tree->setGallary(gallary);
    BVH_Tree->loadData();
    BVH_Tree->prepareSSBOData();
    updateSSBOBuffers();
    loadData = false;
  }

  //  Setup compute shader
  _compute->activateShader();
  glUniform1f(_timeU, snapshot.get_total_time());

  // === CAMERA ====
  if (_cs && _cs->get_main_camera()) {
    _cameraPosition =
        _cs->get_main_camera()->get_entity()->get_world_position();
    _cameraDirection = glm::vec3(0.0f, 8.0f, 4.0f);
    _viewMatrix =
        glm::lookAt(_cameraPosition, _cameraDirection, glm::vec3(0, 1, 0));

    auto s = _wm->get_screen_size();
    _projectionMatrix = glm::perspective(
        glm::radians(_cs->get_main_camera()->get_fov()),
        static_cast<float>(s.x) / static_cast<float>(s.y), _cs->get_main_camera()->get_near(),
        _cs->get_main_camera()->get_far());
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
  glUniform1i(_maximalBouncesU, _bounces);
  glUniform1i(_maxHittableTrianglesU, BVH_Tree->get_numberOfTriangles());
  glUniformMatrix4fv(_projU, 1, GL_FALSE, &_projectionMatrix[0][0]);
  glUniformMatrix4fv(_viewU, 1, GL_FALSE, &_viewMatrix[0][0]);

  auto screen_size = _wm->get_screen_size();
  // int groupsX = (screen_size.x + 16 - 1) / 16;
  // int groupsY = (screen_size.y + 16 - 1) / 16;
  glDispatchCompute(ceil(screen_size.x / 32.0), ceil(screen_size.y / 32.0), 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  // Setup fragment and vertex shader
  _program->activateShader();
  glBindVertexArray(_vao);

  for (auto &&c : _components) {
    c.second->update(snapshot);
  }

  _canvas->update(snapshot);
#endif
}

RenderComponent *RenderSystem::create_component(
    Entity *e, std::optional<AssetManager::Asset> obj_asset,
    std::optional<AssetManager::Asset> mtl_asset,
    std::optional<AssetManager::Asset> shader_asset) {
  LOG("create render component (a1)");
  auto c = create_component_base(e);
  c->set_obj_asset(obj_asset.has_value() ? obj_asset.value() : _da->obj);
  c->set_mtl_asset(mtl_asset.has_value() ? mtl_asset.value() : _da->mtl);
  c->set_shader_asset(shader_asset.has_value() ? shader_asset.value()
                                               : _da->shader);
  return c;
}

RenderComponent *RenderSystem::create_component(
    Entity *e, uuid id, std::optional<AssetManager::Asset> obj_asset,
    std::optional<AssetManager::Asset> mtl_asset,
    std::optional<AssetManager::Asset> shader_asset) {
  LOG("create render component (a2)");
  auto c = create_component_base(e, id);
  c->set_obj_asset(obj_asset.has_value() ? obj_asset.value() : _da->obj);
  c->set_mtl_asset(mtl_asset.has_value() ? mtl_asset.value() : _da->mtl);
  c->set_shader_asset(shader_asset.has_value() ? shader_asset.value()
                                               : _da->shader);
  return c;
}

RenderComponent *RenderSystem::create_component(
    Entity *e, const std::vector<glm::vec3> &vertices,
    const std::vector<glm::vec2> &UV,
    std::optional<AssetManager::Asset> obj_asset,
    std::optional<AssetManager::Asset> mtl_asset,
    std::optional<AssetManager::Asset> shader_asset) {
  LOG("create render component (b1)");
  auto c = create_component_base(e);
  c->set_vertices(vertices);
  c->set_uv(UV);
  c->set_obj_asset(obj_asset.has_value() ? obj_asset.value() : _da->obj);
  c->set_mtl_asset(mtl_asset.has_value() ? mtl_asset.value() : _da->mtl);
  c->set_shader_asset(shader_asset.has_value() ? shader_asset.value()
                                               : _da->shader);

  int programmID = 0;
#if SHOW_UI
  programmID = _program->programID;
#endif
  c->init(programmID);
  return c;
}

RenderComponent *RenderSystem::create_component(
    Entity *e, uuid id, const std::vector<glm::vec3> &vertices,
    const std::vector<glm::vec2> &UV,
    std::optional<AssetManager::Asset> obj_asset,
    std::optional<AssetManager::Asset> mtl_asset,
    std::optional<AssetManager::Asset> shader_asset) {
  LOG("create render component (b2)");
  auto c = create_component_base(e, id);
  c->set_vertices(vertices);
  c->set_uv(UV);
  c->set_obj_asset(obj_asset.has_value() ? obj_asset.value() : _da->obj);
  c->set_mtl_asset(mtl_asset.has_value() ? mtl_asset.value() : _da->mtl);
  c->set_shader_asset(shader_asset.has_value() ? shader_asset.value()
                                               : _da->shader);
  int programmID = 0;
#if SHOW_UI
  programmID = _program->programID;
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

void RenderSystem::updateSSBOBuffers() {
#if SHOW_UI
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo_tree);

  glBufferData(GL_SHADER_STORAGE_BUFFER,
               BVH_Tree->ssboData.size() * sizeof(SSBONodes),
               BVH_Tree->ssboData.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _ssbo_tree);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo_indices);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               BVH_Tree->triIdxData.size() * sizeof(uint32_t),
               BVH_Tree->triIdxData.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _ssbo_indices);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo_vertex);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               BVH_Tree->vertex.size() * sizeof(Vec3Padded),
               BVH_Tree->vertex.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, _ssbo_vertex);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo_mats);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               BVH_Tree->mats.size() * sizeof(Materials), BVH_Tree->mats.data(),
               GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, _ssbo_mats);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo_matsIDX);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               BVH_Tree->matIndx.size() * sizeof(uint32_t),
               BVH_Tree->matIndx.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, _ssbo_matsIDX);
#endif
}

void RenderSystem::update_galary(std::shared_ptr<MeshGallary> mesh_object) {
  #if SHOW_UI
  bool found = false;
  for (auto &c : gallary) {
    if (mesh_object->id == c->id) {
      c->_meshes.clear();
      for (RenderComponentMesh &mesh : mesh_object->_meshes) {
        for (glm::vec3 &vertex : mesh._vertices) {
          vertex = glm::vec3(mesh_object->model * glm::vec4(vertex, 1.0f));
        }
      }

      c->_meshes = mesh_object->_meshes;
      found = true;
      break;
    }
  }

  if (!found) {

    for (RenderComponentMesh &mesh : mesh_object->_meshes) {
      for (glm::vec3 &vertex : mesh._vertices) {
        vertex = glm::vec3(mesh_object->model * glm::vec4(vertex, 1.0f));
      }
    }

    gallary.push_back(mesh_object);
  }
    #endif
}

} // namespace RT
