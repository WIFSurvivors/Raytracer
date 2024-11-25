#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <memory>
#include <vector>

#include "includes/RenderComponent.hpp"
#include "includes/ShaderCompiler.hpp"

namespace core {
/**
 *	RenderSystem class, that is resposible for:
 *	- Window Managing
 *	- Rendering
 *	- Input processing
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
class RenderSystem {
private:
  //  This function is called, whenever the window size changes
  static void framebuffer_size_callback(GLFWwindow *window, int new_width,
                                        int new_height);
  static void processInput(GLFWwindow *window);

  int _scr_height = 800;
  int _scr_width = 800;
  double _mouseX = 0;
  double _mouseY = 0;
  GLuint mouseUniformID;
  GLFWwindow *_window;
  GLuint _vao;

  //As i understand the rendersystem is responsible for the rendering of the different componennts.
  //The View (Camera) and the Projection Matrices have no right to be in the components themselves. In case its not the camera component?
  //Additionally i would like to reallocate the MVP calculation to the respectible shaderes themselfs.
  //They need to be performed each update cycle because maybe the model has changed?
  //

  glm::mat4 _view = glm::lookAt(glm::vec3{0.0f,10.0f,10.0f}, glm::vec3{0.0f,0.0f,0.0f}, glm::vec3{0.0f,1.0f,0.0f});
  float FoV = 60.0f;
  glm::mat4 _projectionMatrix = glm::perspective(glm::radians(FoV), 1.0f, 0.1f, 100.0f);


  //  GLuint _porgramID;
  //  GLuint _computeID;
  //
  //  Not sure about this
  std::unique_ptr<Shader> program;
  std::unique_ptr<Shader> compute;

  int initGLFW();

public:
  //  Jeb, i dont know what to do
  std::unique_ptr<RenderComponent> _component;
  void init();
  void update();
  void destroy();

  //  temporal
  //  we need, this because Render System is responsible for the window and
  //  input handling
  void render();
};

} // namespace core