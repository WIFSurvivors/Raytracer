#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <GL/gl.h>

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

		std::vector<glm::vec3> vertecies = {
			glm::vec3{-0.5f, -0.5f, 0.0f},
			glm::vec3{0.5f, -0.5f, 0.0f},
			glm::vec3{0.0f, 0.5f, 0.0f}
		};

		std::vector<glm::vec2> UV = {
			glm::vec2{0.0f, 0.00f},
			glm::vec2{1.0f, 0.0f},
			glm::vec2{1.0f, 1.0f}
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
}
