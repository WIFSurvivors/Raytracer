#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"


#include "RenderComponent.h"

namespace core {
	
	void RenderComponent::init(GLuint programID) {
		// TODO:
		// - Understand the index of the generic vertex attribute (glVertexAttribPointer())
		// - If we gonna have more render components index has to be different for each right???
		// - Move glENableVertexAttribArray to the update() may be
		// - Do we really need a vertex buffer for UVs
		// - fix warning for #include "glm/gtc/matrix_transform.hpp"
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		// Loads the vertex data to VAO. Requires:
		// - size of the data, number_of_vertices*size_of_one_vertex
		// - pointer to data
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		// Specifies the layout of vertex buffer. Requires:
		// - The stride, meaning the byte offset between vertex attributes, e.g. 3 (because xyz) * sizeof(float)   
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &_uvVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _uvVBO);
		glBufferData(GL_ARRAY_BUFFER, UV.size() * sizeof(glm::vec2), UV.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(1);

		setTextures();

		textUniformID = glGetUniformLocation(programID, "text");
		mvpUniformID = glGetUniformLocation(programID, "MVP");

	}

	void RenderComponent::update() {
		// TODO:
		// - Better MVP calculation (e.g. we don't need to call glm::mat4(1) every time)
		// Calculation for the Model Matrix
		_translationMatrix = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 0.0f));
		_scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(1, 1, 1));
		
		glm::vec3 rotationAxis = glm::vec3(1.0f, 1.0f, 0.0f);
		float angle = 130.0f; 
		_rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(angle), rotationAxis);
		_modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;

		// Calculation for the Camera
		glm::vec3 cameraPosition = glm::vec3(0.0f, 2.0f, 4.0f);
		glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		_viewMatrix = glm::lookAt(cameraPosition, cameraDirection, glm::vec3(0,1,0));

		float FoV = 60.0f;
		_projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f/3.0f, 0.1f, 100.0f);

		glm::mat4 MVP = _projectionMatrix * _viewMatrix * _translationMatrix;
		glUniform1i(textUniformID,  0);
		// GL_FALSE is for transpose
		glUniformMatrix4fv(mvpUniformID, 1, GL_FALSE, &MVP[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, nvertices);
	}

	void RenderComponent::destroy() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_uvVBO);
	}

	void RenderComponent::setTextures() {
		// Generate n = 1 texture IDs
		glGenTextures(1, &_textureID);

		// Activate Texture unit GL_TEXTURE0 
		glActiveTexture(GL_TEXTURE0);

		// Binds new OpenGL texture to the TextureID
		// It means all future texture functions will modify specified texture 
		glBindTexture(GL_TEXTURE_2D, _textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		// Loads the texture data "NULL" to OpenGL 
		// TODO
		// For now it takes 800 800 as screen size, but later it should be as big as texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);

		// Specifies the mipmap level = 0 of the texture 
		glBindImageTexture(0, _textureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, TextureID);
	}
} 
