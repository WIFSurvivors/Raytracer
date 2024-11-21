#include "includes/Entity.hpp"
#include "includes/EntitySystem.hpp"
#include "boost/uuid/uuid_io.hpp"
#include <iostream>

std::shared_ptr<Entity> EntitySystem::create_entity(const std::string &name,
                                                    uuid id) {
  return create_entity(name, id, nullptr);
}

std::shared_ptr<Entity>
EntitySystem::create_entity(const std::string &name, uuid id,
                            std::shared_ptr<Entity> parent) {
  auto e = Entity::create(name, id, parent);
  _entities[id] = e;
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
  if (auto t = static_cast<Entity *>(e)) { // neccessary???
    auto it =
        std::find_if(_entities.begin(), _entities.end(), [t](const auto &val) {
          return val.second.lock().get() == t;
        });
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