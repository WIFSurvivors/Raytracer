#pragma once

#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include "includes/AssetManager.hpp"
#include "includes/utility/obj_loader.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>

struct FrameSnapshot;

/**
 *    RenderComponent class:
 *
 *    TODO:
 *    - Do we store vertices in Render Component???
 *     - if no, then Render Components can be allocated on the stack (i am not
 *sure about this)
 *    - Move view matrix and projection matrix somewhere else
 *    - implement setVertices()
 *    - implement beter set Texture
 *  - Do we need _uvVBO and UV???
 *  - Where do we define the Uniforms??? (I dont like it that it is in the
 *RenderComponent)
 *  - implement better way to store number of vertices
 */
struct RenderComponent : public IComponent {
  RenderComponent(uuid id, Entity *e);
  RenderComponent(uuid id, Entity *e, GLuint programID,
                  const std::vector<glm::vec3> &vertices,
                  const std::vector<glm::vec2> &UV);
  virtual ~RenderComponent();

  void update(const FrameSnapshot& snapshot) override;

  // in theory not required? entity controls the position of an object
  // update modelMatrix based on the entity instead
  void translate(glm::vec3 dir);

  inline void set_vertices(const std::vector<glm::vec3> &vertices) {
#if SHOW_UI
    _vertices = vertices;
    _nvertices = vertices.size();
#endif
  }
  inline void set_uv(const std::vector<glm::vec2> &UV) {
#if SHOW_UI
    _uv = UV;
#endif
  }
  void init(GLuint programID);

  inline const GLuint get_vbo() { return _vbo; }
  inline const GLuint get_textureID() { return _textureID; }
  inline const GLuint get_uvVBO() { return _textureID; }

  inline const AssetManager::Asset  get_obj_asset() const { return *_obj_asset; }
  inline void set_obj_asset(AssetManager::Asset obj_asset) { 
    objl::Loader loader;
    loader.LoadFile(obj_asset.get_path().string());
    std::vector<glm::vec3> vertices;
    for (const auto& vertex : loader.LoadedVertices) {
        vertices.push_back(glm::vec3(vertex.Position.x, vertex.Position.y, vertex.Position.z));
    }
    //set_vertices(vertices);
    for(const auto &ver: vertices) {
      std::cout << ver.x << " " << ver.y << " " << ver.z << std::endl;
    }
    _obj_asset = std::make_shared<AssetManager::Asset>(obj_asset); }
  inline void set_obj_asset(uuid obj_uuid) {  
    _obj_asset->set_uuid(obj_uuid); 
        objl::Loader loader;
    loader.LoadFile(_obj_asset->get_path().string());
    std::vector<glm::vec3> vertices;
    for (const auto& vertex : loader.LoadedVertices) {
        vertices.push_back(glm::vec3(vertex.Position.x, vertex.Position.y, vertex.Position.z));
    }
    set_vertices(vertices);
    }
  inline void set_obj_asset(std::filesystem::path obj_path) {
        objl::Loader loader;
    loader.LoadFile(obj_path.string());
    std::vector<glm::vec3> vertices;
    for (const auto& vertex : loader.LoadedVertices) {
        vertices.push_back(glm::vec3(vertex.Position.x, vertex.Position.y, vertex.Position.z));
    }
    for( const auto& mat: loader.LoadedMaterials) {
      std::cout << mat.name << std::endl;
    }
    set_vertices(vertices);
    _obj_asset->set_path(obj_path);  }

  inline const AssetManager::Asset get_mtl_asset() const { return *_mtl_asset; }
  inline void set_mtl_asset(AssetManager::Asset mtl_asset) { _mtl_asset = std::make_shared<AssetManager::Asset>(mtl_asset); }
  inline void set_mtl_asset(std::filesystem::path mtl_path) {_mtl_asset->set_path(mtl_path);  }
  inline void set_mtl_asset(uuid mtl_uuid) { _mtl_asset->set_uuid(mtl_uuid); }

  inline const AssetManager::Asset get_shader_asset() const { return *_shader_asset; }
  inline void set_shader_asset(AssetManager::Asset shader_asset) { _shader_asset = std::make_shared<AssetManager::Asset>(shader_asset); }
  inline void set_shader_asset(std::filesystem::path shader_path) {_shader_asset->set_path(shader_path);  }
  inline void set_shader_asset(uuid shader_uuid) { _shader_asset->set_uuid(shader_uuid); }
  #if SHOW_UI
  inline void get_vertices(std::vector<glm::vec3> &vertices) { vertices = _vertices; }
  #endif
  void set_from_json(boost::json::object obj) override;

protected:
  boost::json::object to_json_details() const override;

private:
  void destroy();
  

  // void setVertices(); // for now does nothing but later here we can load an
  // object
  void setTextures();

  // is set to true when data is available!
  bool _is_ready = false;
  GLuint _vbo;
  GLuint _textureID;
  GLuint _uvVBO;
  std::shared_ptr<AssetManager::Asset> _obj_asset;
  std::shared_ptr<AssetManager::Asset> _mtl_asset;
  std::shared_ptr<AssetManager::Asset> _shader_asset;
#if SHOW_UI
  glm::mat4 _translationMatrix;
  glm::mat4 _rotationMatrix;
  glm::mat4 _scaleMatrix;
  glm::mat4 _modelMatrix;

  glm::mat4 _viewMatrix;       // shouldn't be here
  glm::mat4 _projectionMatrix; // shouldn't be here

  std::vector<glm::vec3> _vertices = {
      glm::vec3{-1.0f, -1.0f, 0.0f}, glm::vec3{1.0f, -1.0f, 0.0f},
      glm::vec3{1.0f, 1.0f, 0.0f},   glm::vec3{-1.0f, -1.0f, 0.0f},
      glm::vec3{1.0f, 1.0f, 0.0f},   glm::vec3{-1.0f, 1.0f, 0.0f}};

  int _nvertices = 6; // Number of vertices

  std::vector<glm::vec2> _uv = {glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 0.0f},
                                glm::vec2{1.0f, 1.0f}, glm::vec2{0.0f, 0.0f},
                                glm::vec2{1.0f, 1.0f}, glm::vec2{0.0f, 1.0f}};

  GLuint _textU;
  GLuint _modelU;
#endif
};
