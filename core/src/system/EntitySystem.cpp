#include "includes/system/EntitySystem.hpp"
#include "includes/Entity.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include "boost/uuid/uuid_io.hpp"
#include <iostream>

std::shared_ptr<Entity> EntitySystem::create_root(const std::string &name,
                                                  uuid id) {
  auto e = Entity::create(name, id);
  _entities[id] = e.get();
  return e;
}

std::shared_ptr<Entity>
EntitySystem::create_entity(const std::string &name, uuid id,
                            std::shared_ptr<Entity> parent) {
  auto e = Entity::create(name, id, parent);
  _entities[id] = e.get();
  return e;
}

bool EntitySystem::remove(uuid id) {
  auto it = std::find_if(_entities.begin(), _entities.end(),
                         [id](const auto &e) { return e.first == id; });
  if (it != _entities.end()) {
    _entities.erase(it);
    return true;
  }
  return false;
}

bool EntitySystem::remove(Entity *e) {
  auto it = std::find_if(_entities.begin(), _entities.end(),
                         [e](const auto &val) { return val.second == e; });
  if (it != _entities.end()) {
    _entities.erase(it);
    return true;
  }
  return false;
}

void EntitySystem::print() {
  std::cout << "EntitySystem: UUID | Entity Name\n";
  std::cout << _entities.size() << "\n";

  for (auto const &[uuid, e] : _entities) {
    std::cout << "test1\n";
    std::cout << uuid << " | ";
    std::cout << (e != nullptr ? e->get_name() : "unknown") << "\n";
  }
}
EntitySystem::EntitySystem() : System() {
  SimpleLogger::print("entity system 1");
}
