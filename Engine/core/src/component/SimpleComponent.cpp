#include "includes/component/SimpleComponent.hpp"
#include <iostream>
#include <string>

SimpleComponent::SimpleComponent(uuid id, Entity *e)
    : IComponent{id, e}, _value{0} {}

SimpleComponent::SimpleComponent(uuid id, Entity *e, int value)
    : IComponent{id, e}, _value{value} {}

void SimpleComponent::update(const float dt) {}
