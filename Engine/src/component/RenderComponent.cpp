#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "includes/component/RenderComponent.hpp"
#include "includes/utility/NotImplementedError.hpp"

#include "includes/utility/Log.hpp"
#include <glm/gtc/matrix_transform.hpp>

RenderComponent::RenderComponent(uuid id, Entity *e, GLuint programID,
                                 const std::vector<glm::vec3> &vertices,
                                 const std::vector<glm::vec2> &UV)
    : IComponent{id, e, "RenderComponent"} {
#if SHOW_UI
  _vertices = vertices;
  _nvertices = vertices.size();
  _uv = UV;
  _modelMatrix = glm::mat4(1);
  init(programID);
#endif
}

RenderComponent::~RenderComponent() { destroy(); }

void RenderComponent::init(GLuint programID) {
#if SHOW_UI
  Log::message("RenderComponent::init()");
  //  TODO:
  //  - Understand the index of the generic vertex attribute
  //  (glVertexAttribPointer())
  //  - If we gonna have more render components index has to be different for
  //  each right???
  //  - Move glENableVertexAttribArray to the update() may be
  //  - Do we really need a vertex buffer for UVs
  //  - fix warning for #include "glm/gtc/matrix_transform.hpp"
  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  //  Loads the vertex data to VAO. Requires:
  //  - size of the data, number_of_vertices*size_of_one_vertex
  //  - pointer to data
  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3),
               _vertices.data(), GL_STATIC_DRAW);
  //  Specifies the layout of vertex buffer. Requires:
  //  - The stride, meaning the byte offset between vertex attributes, e.g. 3
  //  (because xyz) * sizeof(float)

  glGenBuffers(1, &_uvVBO);

  glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);

  glBufferData(GL_ARRAY_BUFFER, _uv.size() * sizeof(glm::vec2), _uv.data(),
               GL_STATIC_DRAW);

  setTextures();

  _textU = glGetUniformLocation(programID, "text");
  _modelU = glGetUniformLocation(programID, "MVP");
#endif
}

void RenderComponent::update(const float dt) {
#if SHOW_UI
  //  TODO:
  //  - Better MVP calculation (e.g. we don't need to call glm::mat4(1) every
  //  time) Calculation for the Model Matrix
  // glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glUniform1i(_textU, 0);
  //  GL_FALSE is for transpose
  glUniformMatrix4fv(_modelU, 1, GL_FALSE, &_modelMatrix[0][0]);

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

void RenderComponent::destroy() {
#if SHOW_UI
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDeleteBuffers(1, &_vbo);
  glDeleteBuffers(1, &_uvVBO);
#endif
}

void RenderComponent::setTextures() {
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
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 800, 0, GL_RGBA, GL_FLOAT,
               NULL);

  //  Specifies the mipmap level = 0 of the texture
  glBindImageTexture(0, _textureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _textureID);
#endif
}

void RenderComponent::translate(glm::vec3 dir) {
#if SHOW_UI
  _modelMatrix = glm::translate(_modelMatrix, dir);
#endif
}

void RenderComponent::to_json_details(boost::json::object obj) {
  throw NotImplementedError();
}
