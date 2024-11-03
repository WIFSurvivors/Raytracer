#include "component.hpp"
#include <iostream>

component::component(std::weak_ptr<entity> e)
    : _entity{e}, _uuid{component_count++} {}
void component::init() {}

void component::update(float dt) {}

void component::destroy() {}

long component::get_uuid() { return _uuid; }

void component::print() { std::cout << "C: " << _uuid; }
