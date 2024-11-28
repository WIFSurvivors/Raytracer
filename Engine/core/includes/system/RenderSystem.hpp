#pragma once

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
struct RenderSystem {

  RenderSystem(WindowManager *wm);

  void init();
  void update(const float dt); // represents render
  void destroy();

  //  temporal
  //  we need, this because Render System is responsible for the window and
  //  input handling
  void render();
  std::unique_ptr<RenderComponent> _component;
  std::unique_ptr<Shader> program;

private:
  WindowManager *_wm;


  GLuint mouseUniformID; // a bit cringe... but it stays here for now
  GLuint _vao;
  std::vector<glm::vec3> v = {glm::vec3{-0.5f, -0.5f, 0.0f},
                              glm::vec3{0.5f, -0.5f, 0.0f},
                              glm::vec3{0.0f, 0.5f, 0.0f}};

  //  GLuint _porgramID;
  //  GLuint _computeID;
  //
  //  Not sure about this

  std::unique_ptr<Shader> compute;
};

