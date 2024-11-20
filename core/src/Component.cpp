#include "includes/Component.hpp"
#include <iostream>

#include "includes/Entity.hpp"

Component::Component(uuid id, std::weak_ptr<Entity> e) : _uuid{id}, _entity{e} {
  assert(e.lock() == nullptr, "Entity is a nullptr");
  e.lock()->add_component(this);
}
void Component::init() {}

void Component::update(float dt) {}

void Component::destroy() {}

// component::~component() { std::cout << "~component()\n"; }

uuid Component::get_uuid() { return _uuid; }
