#pragma once

#include "includes/system/System.hpp"
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
#include <map>
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
struct RenderSystem : public System {

  explicit RenderSystem(WindowManager *wm);

  void init();
  void update(const float dt); // represents render
  void destroy();

  Component *create_component(uuid id, Entity *e) override;
  RenderComponent *create_component(uuid id, Entity *e,
                                    const std::vector<glm::vec3> &vertices,
                                    const std::vector<glm::vec2> &UV);

  bool remove(Component *c) override;
  bool remove(uuid uuid) override;

  void print();

private:
  WindowManager *_wm;

  GLuint _vao;
  int _nfaces = 0;
  GLuint _facesSSBO = -1;
  int _ncomponents = 0;
  GLuint _materialSSBO = -1;

  std::unique_ptr<Shader> compute;
  std::unique_ptr<Shader> program;

  glm::vec3 _cameraPosition;
  glm::vec3 _cameraDirection;
  float _fov;
  glm::mat4 _viewMatrix;
  glm::mat4 _projectionMatrix;
  GLuint _timeU;
  GLuint _cameraU;
  GLuint _projU;
  GLuint _viewU;

  std::unique_ptr<Canvas> _canvas;
  std::map<uuid, std::unique_ptr<RenderComponent>> _components{};

  void loadSSBO();
};
