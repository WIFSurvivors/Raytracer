#pragma once

#include "includes/System.hpp"
#include "boost/uuid/uuid.hpp"
#include <memory>
#include <map>
#include <string>

typedef boost::uuids::uuid uuid;

struct Entity;

struct EntitySystem : public System {
  EntitySystem() = default;

  std::shared_ptr<Entity> create_entity(const std::string &name,
                                        uuid id) override;

  std::shared_ptr<Entity> create_entity(std::string name, uuid id,
                                        std::shared_ptr<Entity> parent);

  bool remove(uuid id) override;
  bool remove(Entity *e) override;

  void print();

private:
  std::map<uuid, std::weak_ptr<Entity>> _entities{};
};
