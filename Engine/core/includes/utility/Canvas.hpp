#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


struct Canvas{
  Canvas(GLuint programID);
  void init(GLuint programID);
  void update(float dt);
  void destroy();

private:
  GLuint _vbo;
  GLuint _textureID;
  GLuint _uvVBO;

  glm::mat4 _modelMatrix;

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

  void setTextures();
};
