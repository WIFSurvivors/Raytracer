#include "includes/SimpleSystem.hpp"
#include "includes/Entity.hpp"
#include "includes/SimpleComponent.hpp"
#include "boost/uuid/uuid_io.hpp"
#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>

SimpleComponent *SimpleSystem::create_component(uuid id,
                                                std::shared_ptr<Entity> e) {
  return create_component(id, e, 0);
}

SimpleComponent *
SimpleSystem::create_component(uuid id, std::shared_ptr<Entity> e, int value) {
  _components[id] = std::make_unique<SimpleComponent>(e, id, value);
  auto ptr = _components[id].get();
  e->add_component(ptr);
  return ptr;
}

bool SimpleSystem::remove(uuid id) {
  auto it =
      std::find_if(_components.begin(), _components.end(),
                   [id](SimpleComponent *c) { return c->get_uuid() == id; });
  if (it != _components.end()) {
    _components.erase(it);
    return true;
  }
  return false;
}

bool SimpleSystem::remove(Component *c) {
  if (auto t = static_cast<SimpleComponent *>(c)) {
    auto it = std::find(_components.begin(), _components.end(), t);
    if (it != _components.end()) {
      _components.erase(it);
      return true;
    }
  }
  return false;
}

void SimpleSystem::clear() { _components.clear(); }

void SimpleSystem::update(const float dt) {
  for (auto &&c : _components) {
    c.second->update(dt);
  }
}

void SimpleSystem::print_all_components() {
  std::cout << "CURRENT SIMPLE SYSTEM COMPONENTS:\n";
  std::cout << "c.uuid | c.adr          | c.value | c.Entity-uuid:\n";
  for (auto &&c : _components) {
    std::cout << "- " << c.first << " | " << c.second << " | "
              << c.second->get_value()
              << " | e: " << (c.second->_entity.lock())->get_uuid() << "\n";
  }
}
