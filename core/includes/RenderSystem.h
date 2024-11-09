#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GL/gl.h>
#include <memory>

#include "RenderComponent.h"
#include "ShaderCompiler.h"

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
	class RenderSystem{
	private:
		// This function is called, whenever the window size changes
		void static framebuffer_size_callback(GLFWwindow *window, int new_width, int new_height);
		void static processInput(GLFWwindow *window);

		int _scr_height = 800;
		int _scr_width = 800;
		double _mouseX = 0;
		double _mouseY = 0;
		GLuint mouseUniformID;
		GLFWwindow *_window;
		GLuint _vao;
		std::vector<glm::vec3> v = {glm::vec3{-0.5f, -0.5f, 0.0f},
                                       glm::vec3{0.5f, -0.5f, 0.0f},
                                       glm::vec3{0.0f, 0.5f, 0.0f}};

		// GLuint _porgramID;
		// GLuint _computeID;
		//
		// Not sure about this
		std::unique_ptr<Shader> program;
		std::unique_ptr<Shader> compute;


		int initGLFW();



	public:
		// Jeb, i dont know what to do
		std::unique_ptr<RenderComponent> _component;
		void init();
		void update();
		void destroy();

		// temporal
		// we need, this because Render System is responsible for the window and input handling
		void render();
	};


}
