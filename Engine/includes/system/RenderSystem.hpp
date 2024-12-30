#pragma once

#include "includes/system/System.hpp"
#include "includes/system/CameraSystem.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/ShaderCompiler.hpp"
#include "includes/WindowManager.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <memory>
#include <vector>
#include <map>
#include <string>

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

  RenderSystem(WindowManager *wm, CameraSystem *cs);

  void init();
  void update(const float total_time); // represents render
  void destroy();

  RenderComponent *create_component(uuid id, Entity *e);
  RenderComponent *create_component(uuid id, Entity *e,
                                    const std::vector<glm::vec3> &vertices,
                                    const std::vector<glm::vec2> &UV);

  inline std::string get_system_name() const override {
    return "Render System";
  }

  //  temporal
  //  we need, this because Render System is responsible for the window and
  //  input handling
  // void render();
  // std::unique_ptr<RenderComponent> _component;
  void print() override;

private:
  WindowManager *_wm;
  CameraSystem *_cs;
  
  using typename System::uuid;

#if SHOW_UI
  std::unique_ptr<Shader> program;

  // GLuint mouseUniformID; // a bit cringe... but it stays here for now
  GLuint ssbo_tree;
  GLuint ssbo_triangle;
  GLuint ssbo_vertex;
  GLuint ssbo_indices;
  GLuint ssbo_mats;
  GLuint ssbo_matsIDX;
  GLuint _vao;
  std::vector<glm::vec3> v = {glm::vec3{-0.5f, -0.5f, 0.0f},
                              glm::vec3{0.5f, -0.5f, 0.0f},
                              glm::vec3{0.0f, 0.5f, 0.0f}};
  glm::vec3 _cameraPosition;
  glm::vec3 _cameraDirection;
  float _fov;
  glm::mat4 _viewMatrix;
  glm::mat4 _projectionMatrix;
  GLuint _timeU;
  GLuint _cameraU;
  GLuint _projU;
  GLuint _viewU;

  std::unique_ptr<Shader> compute;
#endif
};
