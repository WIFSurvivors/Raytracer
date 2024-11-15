#include "includes/SimpleSystem.hpp"
#include "includes/Entity.hpp"
#include "includes/SimpleComponent.hpp"
#include <iostream>
#include <memory>
#include <utility>
#include <type_traits>

Component *SimpleSystem::create_component(int64_t uuid) {}

// probably inefficent because std::move doesn't move the pointer underneath
SimpleComponent *SimpleSystem::create_component(std::shared_ptr<Entity> e) {
  _components[component_count] = std::make_unique<SimpleComponent>(e);
  auto c = _components[component_count].get();
  e->add_component(c);
  return c;
}

SimpleComponent *SimpleSystem::create_component(std::shared_ptr<Entity> e,
                                                int value) {
  _components[component_count] = std::make_unique<SimpleComponent>(e, value);
  e->add_component(_components[component_count].get());
  return _components[component_count++].get();
}

bool SimpleSystem::remove(int64_t uuid) {
  auto it = std::find_if(
      _components.begin(), _components.end(),
      [uuid](SimpleComponent *c) { return c->get_uuid() == uuid; });
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
