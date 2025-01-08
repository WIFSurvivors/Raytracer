#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "includes/utility/Canvas.hpp"
#include "includes/utility/FrameSnapshot.hpp"

#include "includes/utility/Log.hpp"
#include <glm/gtc/matrix_transform.hpp>
namespace RT
{
    

struct FrameSnapshot;

Canvas::Canvas(GLuint programID, glm::ivec2 screen_size) {
#if SHOW_UI
  // _vertices = vertices;
  // _nvertices = vertices.size();
  // _uv = UV;
  init(programID, screen_size);
#endif
}

Canvas::~Canvas() { destroy(); }

void Canvas::init(GLuint programID, glm::ivec2 screen_size) {
  LOG("RenderComponent::init()");
#if SHOW_UI
  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3),
               _vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &_uvVBO);
  glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);
  glBufferData(GL_ARRAY_BUFFER, _uv.size() * sizeof(glm::vec2), _uv.data(),
               GL_STATIC_DRAW);

  setTextures(screen_size);

  _textU = glGetUniformLocation(programID, "text");
  // _modelU = glGetUniformLocation(programID, "MVP");
#endif
  _is_ready = true;
}

void Canvas::update(const FrameSnapshot& snapshot) {
	if(!_is_ready) return;
#if SHOW_UI

  glUniform1i(_textU, 0);

  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        reinterpret_cast<void *>(0));

  glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        reinterpret_cast<void *>(0));

  glDrawArrays(GL_TRIANGLES, 0, _nvertices);
  // glBindVertexArray(0);
#endif
}

void Canvas::destroy() {
#if SHOW_UI
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDeleteBuffers(1, &_vbo);
  glDeleteBuffers(1, &_uvVBO);
#endif
}

void Canvas::setTextures(glm::ivec2 screen_size) {
#if SHOW_UI
  //  Generate n = 1 texture IDs
  glGenTextures(1, &_textureID);

  //  Activate Texture unit GL_TEXTURE0
  glActiveTexture(GL_TEXTURE0);

  //  Binds new OpenGL texture to the TextureID
  //  It means all future texture functions will modify specified texture
  glBindTexture(GL_TEXTURE_2D, _textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  //  Loads the texture data "NULL" to OpenGL
  //  TODO
  //  For now it takes 800 800 as screen size, but later it should be as big as
  //  texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screen_size.x, screen_size.y, 0, GL_RGBA, GL_FLOAT,
               NULL);

  //  Specifies the mipmap level = 0 of the texture
  glBindImageTexture(0, _textureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _textureID);
#endif
}
} // namespace RT
