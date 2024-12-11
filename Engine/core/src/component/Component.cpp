#include "includes/component/Component.hpp"
#include "includes/Entity.hpp"
#include <iostream>
#include <cassert>

Component::Component(uuid id, Entity *e) : _uuid{id}, _entity{e} {
  // has to be rewritten for windows apparently?
  // https://en.cppreference.com/w/cpp/error/assert
  assert(e != nullptr && "Entity is a nullptr");
  e->add_component(this);
}

Component::~Component() {
  if (_entity != nullptr) {
    get_entity()->remove_component(this);
  }
}

void Component::update(const float dt) {}

uuid Component::get_uuid() { return _uuid; }
