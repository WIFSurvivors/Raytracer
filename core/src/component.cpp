#include "component.hpp"
#include <iostream>

component::component(std::weak_ptr<entity> e)
    : _entity{e}, _uuid{component_count++} {}
void component::init() {}

void component::update(float dt) {}

void component::destroy() {}

component::~component() { std::cout << "~component()\n"; }

long component::get_uuid() { return _uuid; }

void component::print() { std::cout << "c:ad " << _uuid; }
