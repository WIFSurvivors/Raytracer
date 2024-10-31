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
	 *	Engine class, that is resposible for:
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
	 *	- Do wen really need a header file per class???
	 */
	class Engine {
	private:
		// This function is called, whenever the window size changes
		void static framebuffer_size_callback(GLFWwindow *window, int new_width, int new_height);
		void static processInput(GLFWwindow *window);

		int SCR_HEIGHT = 800;
		int SCR_WIDTH = 800;

		GLFWwindow *window;
		GLuint VBO;
		GLuint VAO;
		GLuint TextureID;


		glm::mat4 TranslationMatrix;
		glm::mat4 RotationMatrix;
		glm::mat4 ScaleMatrix;
		glm::mat4 ModelMatrix;

		glm::mat4 viewMatrix;

		glm::mat4 projectionMatrix;

		std::vector<glm::vec3> vertecies = {
			glm::vec3{-1.0f, -1.0f, 0.0f},
			glm::vec3{1.0f, -1.0f, 0.0f},
			glm::vec3{1.0f, 1.0f, 0.0f},
			glm::vec3{-1.0f, -1.0f, 0.0f},
			glm::vec3{1.0f, 1.0f, 0.0f},
			glm::vec3{-1.0f, 1.0f, 0.0f}
		};

		std::vector<glm::vec2> UV = {
			glm::vec2{0.0f, 0.0f},
			glm::vec2{1.0f, 0.0f},
			glm::vec2{1.0f, 1.0f},
			glm::vec2{0.0f, 0.0f},
			glm::vec2{1.0f, 1.0f},
			glm::vec2{0.0f, 1.0f}
		};

		void terminate();
		void initTextures();
		void render_loop();

		int initGLFW();

	public:
		// This function:
		// - Initializes window (GLFW)
		// - Starts rendering loop
		// - After rendering terminates everything
		void init();
	};
	/**
	 *	Engine class, that is resposible for:
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

		GLFWwindow *_window;
		GLuint _vao;

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
