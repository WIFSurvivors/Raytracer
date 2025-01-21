#pragma once

#include "includes/system/System.hpp"
#include "includes/system/CameraSystem.hpp"
#include "includes/system/LightSystem.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/ShaderCompiler.hpp"
#include "includes/WindowManager.hpp"
#include "includes/utility/Canvas.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <memory>
#include <vector>
#include <string>
#include <optional>

namespace RT {
struct FrameSnapshot;
// #define SHOW_UI true

/**
 *	RenderSystem class, that is resposible for:
 *	- ~~Window Managing~~
 *	- Rendering
 *	- ~~Input processing~~
 *
 *	TODO:
 *	- Differentiate between private and public members
 *	- Move responisbilities to separate classes
 *	- Change init
 *	- May be RAII?
 *	- ERROR system would be good
 *	- Resizing doesn't work
 *	 - Decide what happenes by resizing: scaling or viewport
 *	- Make RenderSystem abstract, so that we can implement children
 *	- Move Window function e.g to InputSystem???
 *	- Jeb, i dont know what to do: implement create_component()
 *	- Do we really need a separate class for shaders
 */
struct RenderSystem : public System<RenderComponent> {
  RenderSystem(std::shared_ptr<UUIDManager> um, WindowManager *wm,
               CameraSystem *cs, LightSystem *ls,
               AssetManager::DefaultAssets *da);
  ~RenderSystem();

  void init();
  void update(const FrameSnapshot &snapshot); // represents render
  void destroy();

  RenderComponent *
  create_component(Entity *e, std::optional<AssetManager::Asset> obj_asset = {},
                   std::optional<AssetManager::Asset> mtl_asset = {},
                   std::optional<AssetManager::Asset> shader_asset = {});
  RenderComponent *
  create_component(Entity *e, uuid id,
                   std::optional<AssetManager::Asset> obj_asset = {},
                   std::optional<AssetManager::Asset> mtl_asset = {},
                   std::optional<AssetManager::Asset> shader_asset = {});
  RenderComponent *
  create_component(Entity *e, const std::vector<glm::vec3> &vertices,
                   const std::vector<glm::vec2> &UV,
                   std::optional<AssetManager::Asset> obj_asset = {},
                   std::optional<AssetManager::Asset> mtl_asset = {},
                   std::optional<AssetManager::Asset> shader_asset = {});
  RenderComponent *
  create_component(Entity *e, uuid id, const std::vector<glm::vec3> &vertices,
                   const std::vector<glm::vec2> &UV,
                   std::optional<AssetManager::Asset> obj_asset = {},
                   std::optional<AssetManager::Asset> mtl_asset = {},
                   std::optional<AssetManager::Asset> shader_asset = {});

  inline void set_bounces(int bounce) { _bounces = bounce; }
  inline int get_bounces() const { return _bounces; }

  inline const std::string get_name() const final { return "Render System"; }

  void updateSSBOBuffers();
  //  temporal
  //  we need, this because Render System is responsible for the window and
  //  input handling
  // void render();
  // std::unique_ptr<RenderComponent> _component;
  void print() override;
  bool remove(uuid id) override;
  void update_galary(std::shared_ptr<MeshGallary> mesh_object);

private:
  WindowManager *_wm;
  CameraSystem *_cs;
  LightSystem *_ls;
  AssetManager::DefaultAssets *_da;
  int _bounces = 4;
  using typename System::uuid;

#if SHOW_UI
  std::unique_ptr<Shader> _program;
  std::unique_ptr<Canvas> _canvas;
  // GLuint mouseUniformID; // a bit cringe... but it stays here for now
  GLuint _ssbo_tree;
  //GLuint _ssbo_triangle;
  GLuint _ssbo_vertex;
  GLuint _ssbo_indices;
  GLuint _ssbo_mats;
  GLuint _ssbo_matsIDX;
  GLuint _vao;

  glm::ivec2 _screen_size;

  glm::vec3 _cameraPosition;
  glm::vec3 _cameraDirection;
  float _fov;

  glm::mat4 _viewMatrix;
  glm::mat4 _projectionMatrix;

  glm::mat4 _modelMatrix_Canvas = glm::mat4(1.0f);
  GLuint _timeU;
  GLuint _cameraU;
  GLuint _projU;
  GLuint _viewU;

  GLuint _ls_active_light_sourcesU;
  GLuint _ls_positionsU;
  GLuint _ls_directionsU;
  GLuint _ls_colorsU;
  GLuint _ls_intensitiesU;

  GLuint _maximalBouncesU;
  GLuint _maxHittableTrianglesU;

  std::unique_ptr<Shader> _compute;
  std::vector<std::shared_ptr<MeshGallary>> gallary;

#endif
bool _loadData;
};
} // namespace RT
