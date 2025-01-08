#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
namespace RT
{
struct FrameSnapshot;

struct Canvas {
  Canvas(GLuint programID, glm::ivec2 screen_size);
  ~Canvas();

  void update(const FrameSnapshot& snapshot);

  void init(GLuint programID, glm::ivec2 screen_size);

  inline const GLuint get_vbo() { return _vbo; }
  inline const GLuint get_textureID() { return _textureID; }
  inline const GLuint get_uvVBO() { return _textureID; }

private:
  void destroy();

 void setTextures(glm::ivec2 screen_size);

  bool _is_ready = false;
  GLuint _vbo;
  GLuint _textureID;
  GLuint _uvVBO;

#if SHOW_UI

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

} // namespace RT
