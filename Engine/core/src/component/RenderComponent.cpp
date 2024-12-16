#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "includes/component/RenderComponent.hpp"

#include "includes/utility/SimpleLogger.hpp"
#include <glm/gtc/matrix_transform.hpp>

RenderComponent::RenderComponent(uuid id, Entity *e)
    : Component{id, e} {
}

void RenderComponent::init() {
}

void RenderComponent::update() {
}

void RenderComponent::destroy() {
}
