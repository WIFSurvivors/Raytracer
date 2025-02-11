#pragma once

#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include "includes/AssetManager.hpp"
#include "includes/utility/obj_loader.hpp"
#include "includes/utility/data_loader.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>

namespace RT {
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

  void update(const FrameSnapshot &snapshot) override;

  // in theory not req uired? entity controls the position of an object
  // update modelMatrix based on the entity instead
  void translate(glm::vec3 dir);

  inline void set_vertices(const std::vector<glm::vec3> &vertices) {
#if SHOW_UI
    _vertices = vertices;
    _nvertices = vertices.size();
#endif
  }

  inline std::vector<RenderComponentMesh> get_meshes() {
#if SHOW_UI
    return _meshes;
#else
    return {};
#endif
  }

  inline void set_translationMatrix(const glm::mat4 translation) {
#if SHOW_UI
    _translationMatrix = translation;
#endif
  }

  inline void set_rotationMatrix(const glm::mat4 rotation) {
#if SHOW_UI
    _rotationMatrix = rotation;
#endif
  }

  inline void set_scaleMatrix(const glm::mat4 scale) {
#if SHOW_UI
    _scaleMatrix = scale;
#endif
  }

  inline void update_ModelMatrix() {
#if SHOW_UI
    _modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
#endif
  }
  inline glm::mat4 get_ModelMatrix() {
#if SHOW_UI
    return _modelMatrix;
#else
    return 0;
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

  inline const AssetManager::Asset get_obj_asset() const { return *_obj_asset; }

  inline void set_obj_asset(AssetManager::Asset obj_asset) {
#if SHOW_UI
    objl::Loader loader;
	if(!loader.LoadFile(obj_asset.get_path().string())){
		LOG_ERROR("invalid .obj file");
		return;
	}

    std::vector<glm::vec3> vertices;

    /*for (const auto &vertex : loader.LoadedVertices) {*/
    /*  vertices.push_back(*/
    /*      glm::vec3(vertex.Position.x, vertex.Position.y,
     * vertex.Position.z));*/
    /*}*/
    _meshes.clear();
    for (const auto &mesh : loader.LoadedMeshes) {
      RenderComponentMesh renderMesh{};
      renderMesh._vertices.clear();
      renderMesh._indices.clear();
      for (const auto &vertex : mesh.Vertices) {

        renderMesh._vertices.push_back(

            glm::vec3(vertex.Position.x, vertex.Position.y, vertex.Position.z));

        renderMesh._normals.push_back(
            glm::vec3(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z));
      }
      renderMesh._indices.insert(renderMesh._indices.end(),
                                 mesh.Indices.begin(), mesh.Indices.end());

      renderMesh.MeshMaterial = mesh.MeshMaterial;

      _meshes.push_back(renderMesh);
	  std::cout << "FRICKING NS: " << renderMesh.MeshMaterial.Ns << std::endl;
	  std::cout << "FRICKING Illum: " << renderMesh.MeshMaterial.illum<< std::endl;
    }

#endif
    _obj_asset = std::make_shared<AssetManager::Asset>(obj_asset);
  }

  inline void set_obj_asset(uuid obj_uuid) {
    _obj_asset->set_uuid(obj_uuid);
#if SHOW_UI
    objl::Loader loader;
    loader.LoadFile(_obj_asset->get_path().string());
    _meshes.clear();
    for (const auto &mesh : loader.LoadedMeshes) {
      RenderComponentMesh renderMesh{};
      renderMesh._vertices.clear();
      renderMesh._indices.clear();
      for (const auto &vertex : mesh.Vertices) {
        renderMesh._vertices.push_back(
            glm::vec3(vertex.Position.x, vertex.Position.y, vertex.Position.z));

        renderMesh._normals.push_back(
            glm::vec3(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z));
      }
      renderMesh._indices.insert(renderMesh._indices.end(),
                                 mesh.Indices.begin(), mesh.Indices.end());
      renderMesh.MeshMaterial = mesh.MeshMaterial;
      _meshes.push_back(renderMesh);
    }
#endif
  }
  inline void set_obj_asset(std::filesystem::path obj_path) {
    objl::Loader loader;
#if SHOW_UI
    loader.LoadFile(obj_path.string());

    _meshes.clear();
    for (const auto &mesh : loader.LoadedMeshes) {
      RenderComponentMesh renderMesh{};
      renderMesh._vertices.clear();
      renderMesh._indices.clear();
      for (const auto &vertex : mesh.Vertices) {
        renderMesh._vertices.push_back(
            glm::vec3(vertex.Position.x, vertex.Position.y, vertex.Position.z));

        renderMesh._normals.push_back(
            glm::vec3(vertex.Normal.x, vertex.Normal.y, vertex.Normal.z));
      }
      renderMesh._indices.insert(renderMesh._indices.end(),
                                 mesh.Indices.begin(), mesh.Indices.end());
      renderMesh.MeshMaterial = mesh.MeshMaterial;
      _meshes.push_back(renderMesh);
    }
#endif
    _obj_asset->set_path(obj_path);
  }


#if SHOW_UI
  inline std::vector<glm::vec3> get_vertices() { return _vertices; }
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
#if SHOW_UI
  glm::mat4 _translationMatrix;
  glm::mat4 _rotationMatrix;
  glm::mat4 _scaleMatrix;
  glm::mat4 _modelMatrix;

  glm::mat4 _viewMatrix;       // shouldn't be here
  glm::mat4 _projectionMatrix; // shouldn't be here

  std::vector<glm::vec3> _vertices;
  std::vector<RenderComponentMesh> _meshes;
  int _nvertices = 0; // Number of vertices

  std::vector<glm::vec2> _uv = {
      glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 0.0f}, glm::vec2{1.0f, 1.0f},
      glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}, glm::vec2 { 0.0f, 1.0f }};

  GLuint _textU;
  GLuint _modelU;
#endif
};
}; // namespace RT
