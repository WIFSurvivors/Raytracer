#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "includes/component/RenderComponent.hpp"

#include "glm/fwd.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include <glm/gtc/matrix_transform.hpp>

RenderComponent::RenderComponent(uuid id, Entity *e)
    : Component{id, e} {
}

void RenderComponent::init() {
}

void RenderComponent::update(int counter, GLuint modelSSBO) {
    glBufferSubData(modelSSBO, counter * sizeof(glm::mat4), sizeof(glm::mat4), _entity->_modelMatrix);
}

void RenderComponent::destroy() {
}

int RenderComponent::loadSSBO(int counter, int offset, GLuint facesSSBO, GLuint materialSSBO) {
    glBufferSubData(facesSSBO, offset, _nfaces * sizeof(Face), _faces.data());

    glBufferSubData(materialSSBO, counter * sizeof(Material), sizeof(Material), &_material);
}
