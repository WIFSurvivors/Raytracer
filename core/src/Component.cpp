#include "includes/Component.hpp"
#include <iostream>

Component::Component(std::weak_ptr<Entity> e)
    : _entity{e}, _uuid{component_count++} {}
void Component::init() {}

void Component::update(float dt) {}

void Component::destroy() {}

// component::~component() { std::cout << "~component()\n"; }

int64_t Component::get_uuid() { return _uuid; }

void Component::print() { std::cout << "c:ad " << _uuid; }
