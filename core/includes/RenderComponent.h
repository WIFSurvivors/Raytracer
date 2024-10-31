#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace core {
	/**
	 *	RenderComponent class:
	 *
	 *	TODO:
	 *	- Do we store vertices in Render Component???
	 *	 - if no, then Render Components can be allocated on the stack (i am not sure about this)
	 *	- Move view matrix and projection matrix somewhere else
	 *	- implement setVertices()
	 *	- implement beter set Texture
	 *  - Do we need _uvVBO and UV??? 
	 *  - Where do we define the Uniforms??? (I dont like it that it is in the RenderComponent)
	 */
	class RenderComponent{
	private:
		GLuint _vbo;
		GLuint _textureID;
		GLuint _uvVBO;

		glm::mat4 _translationMatrix;
		glm::mat4 _rotationMatrix;
		glm::mat4 _scaleMatrix;
		glm::mat4 _modelMatrix;

		glm::mat4 _viewMatrix;
		glm::mat4 _projectionMatrix;

		std::vector<glm::vec3> vertices = {
			glm::vec3{-1.0f, -1.0f, 0.0f},
			glm::vec3{1.0f, -1.0f, 0.0f},
			glm::vec3{1.0f, 1.0f, 0.0f},
			glm::vec3{-1.0f, -1.0f, 0.0f},
			glm::vec3{1.0f, 1.0f, 0.0f},
			glm::vec3{-1.0f, 1.0f, 0.0f}
		};
		
		int nvertices = 6;

		std::vector<glm::vec2> UV = {
			glm::vec2{0.0f, 0.0f},
			glm::vec2{1.0f, 0.0f},
			glm::vec2{1.0f, 1.0f},
			glm::vec2{0.0f, 0.0f},
			glm::vec2{1.0f, 1.0f},
			glm::vec2{0.0f, 1.0f}
		};

		GLuint textUniformID;
		GLuint mvpUniformID;
		// for now does nothing but later here we can load an object
		void setVertices();
		void setTextures();
	public:
		void init(GLuint programID);
		void update();
		void destroy();
	};
}
