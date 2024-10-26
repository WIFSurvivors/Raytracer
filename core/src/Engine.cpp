#include "ShaderCompiler.h"
#include "Engine.h"


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace core{
	/**
	 *	TODO:
	 *	- Refactor render loop
	 *	  - e.g. Call it something else
	 *	  - Separate other functionality to the functions
	 *	- Refactor init
	 *
	 */
	void Engine::processInput(GLFWwindow *window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void Engine::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
		// make sure the viewport matches the new window dimensions; 
		// note that width and height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	void Engine::init() {

		if (!initGLFW()) throw std::runtime_error{"Failed to Initialize Engine"};
		
		render_loop();
		
		terminate();
	}

	void Engine::terminate() {
		// Do Deletion Here
		glfwTerminate();
	}

	void Engine::initTextures() {
		// Generate n = 1 texture IDs
		glGenTextures(1, &TextureID);

		// Activate Texture unit GL_TEXTURE0 
		glActiveTexture(GL_TEXTURE0);

		// Binds new OpenGL texture to the TextureID
		// It means all future texture functions will modify specified texture 
		glBindTexture(GL_TEXTURE_2D, TextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		// Loads the texture data "NULL" to OpenGL 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);

		// Specifies the mipmap level = 0 of the texture 
		glBindImageTexture(0, TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, TextureID);
	}

	void Engine::render_loop() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		// Activates VAO
		glBindVertexArray(VAO);
		// Binds VBO to VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Loads the vertex data to VAO. Requires:
		// - size of the data, number_of_vertices*size_of_one_vertex
		// - pointer to data
		glBufferData(GL_ARRAY_BUFFER, vertecies.size() * sizeof(glm::vec3), vertecies.data(), GL_STATIC_DRAW);

		// Specifies the layout of vertex buffer. Requires:
		// - The stride, meaning the byte offset between vertex attributes, e.g. 3 (because xyz) * sizeof(float)   
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		initTextures();

		// Initializes vertex and fragment shaders
		Shader simpleShader {
			std::make_pair(GL_VERTEX_SHADER, "../shaders/vertexshader.glsl"),
			std::make_pair(GL_FRAGMENT_SHADER, "../shaders/fragmentshader.glsl")
		};
		simpleShader.activateShader();

		// Specifies Uniform Buffer Location "text" where the sahder info could be found  
		glUniform1i(glGetUniformLocation(simpleShader.programID, "text"), 0);
		
		// The same as vertex VBO but for textures
		GLuint Tex_VBO;
		glGenBuffers(1, &Tex_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, Tex_VBO);
		glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), textureCoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(1);

		Shader computeShader {
			std::make_pair(GL_COMPUTE_SHADER, "../shaders/computeshader.glsl")
		};
		
		// RENDER LOOP
		while (!glfwWindowShouldClose(window)) {
			glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
			// Input
			processInput(window);

			// Render

			// Specifies the background color1
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			computeShader.activateShader();
			
			// Self explanatory
			glUniform1f(glGetUniformLocation(computeShader.programID, "time"), glfwGetTime());
			glUniform3fv(glGetUniformLocation(computeShader.programID, "v0"), 1, glm::value_ptr(vertecies[0]));
			glUniform3fv(glGetUniformLocation(computeShader.programID, "v1"), 1, glm::value_ptr(vertecies[1]));
			glUniform3fv(glGetUniformLocation(computeShader.programID, "v2"), 1, glm::value_ptr(vertecies[2]));
			glUniform2fv(glGetUniformLocation(computeShader.programID, "imageSize"), 1, glm::value_ptr(glm::vec2(800, 800)));

			// Dispateches the compute shader with SCR_WIDTH*SCR_HEIGHT*1 = number of work groups
			glDispatchCompute(SCR_WIDTH, SCR_HEIGHT, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
			simpleShader.activateShader();
			glDrawArrays(GL_TRIANGLES, 0, 3);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse
			// moved etc.)
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	int Engine::initGLFW() {

		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		// glfw window creation
		// --------------------
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
		if (window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return 0;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			return 0;
		}
		return 1;
	}}

