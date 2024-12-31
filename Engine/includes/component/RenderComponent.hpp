#pragma once

#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

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
