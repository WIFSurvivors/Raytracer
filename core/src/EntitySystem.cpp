#include "includes/Entity.hpp"
#include "includes/EntitySystem.hpp"

#include <iostream>

std::shared_ptr<Entity> EntitySystem::create_entity(const std::string &name,
                                                    int64_t uuid) {
  return create_entity(name, uuid, nullptr);
}

std::shared_ptr<Entity>
EntitySystem::create_entity(std::string name, int64_t uuid,
                            std::shared_ptr<Entity> parent) {
  auto e = Entity::create(name, uuid, parent);
  _entities[uuid] = e;
  return e;
}

bool EntitySystem::remove(int64_t uuid) {
  auto it = std::find_if(_entities.begin(), _entities.end(),
                         [uuid](Entity *e) { return e->get_uuid() == uuid; });
  if (it != _entities.end()) {
    _entities.erase(it);
    return true;
  }
  return false;
}

bool EntitySystem::remove(Entity *e) {
  if (auto t = static_cast<Entity *>(e)) {
    auto it = std::find(_entities.begin(), _entities.end(), t);
    if (it != _entities.end()) {
      _entities.erase(it);
      return true;
    }
  }
  return false;
}

void EntitySystem::print() {
  std::cout << "EntitySystem: UUID | Entity Name\n";
  for (auto const &[uuid, e] : _entities) {
    auto e_locked = e.lock();
    if (e_locked)
      std::cout << uuid << " | " << e_locked->get_name() << "\n";
  }
}