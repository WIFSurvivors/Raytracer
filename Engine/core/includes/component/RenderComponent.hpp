#pragma once

#include "includes/component/Component.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


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
class RenderComponent  : public virtual Component{
private:
  GLuint _vbo;
  GLuint _textureID;
  GLuint _uvVBO;

  glm::mat4 _translationMatrix;
  glm::mat4 _rotationMatrix;
  glm::mat4 _scaleMatrix;
  glm::mat4 _modelMatrix;

  glm::mat4 _viewMatrix;       // shouldn't be here
  glm::mat4 _projectionMatrix; // shouldn't be here

  std::vector<glm::vec3> vertices = {
      glm::vec3{-1.0f, -1.0f, 0.0f}, glm::vec3{1.0f, -1.0f, 0.0f},
      glm::vec3{1.0f, 1.0f, 0.0f},   glm::vec3{-1.0f, -1.0f, 0.0f},
      glm::vec3{1.0f, 1.0f, 0.0f},   glm::vec3{-1.0f, 1.0f, 0.0f}};

  int nvertices = 6; // Number of vertices

  std::vector<glm::vec2> UV = {glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 0.0f},
                               glm::vec2{1.0f, 1.0f}, glm::vec2{0.0f, 0.0f},
                               glm::vec2{1.0f, 1.0f}, glm::vec2{0.0f, 1.0f}};

  GLuint textUniformID;
  GLuint mvpUniformID;

  // void setVertices(); // for now does nothing but later here we can load an
  // object
  void setTextures();

public:
// RenderComponent();
  void init(GLuint programID);
  void update(GLuint VAO);
  void destroy();
};
