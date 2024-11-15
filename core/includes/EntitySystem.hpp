#pragma once

#include "includes/System.hpp"
#include <memory>
#include <map>
#include <string>

struct Entity;

struct EntitySystem : public System {
  EntitySystem() = default;

  std::shared_ptr<Entity> create_entity(const std::string &name,
                                        int64_t uuid) override;

  std::shared_ptr<Entity> create_entity(std::string name, int64_t uuid,
                                        std::shared_ptr<Entity> parent);

  bool remove(int64_t uuid) override;
  bool remove(Entity *e) override;

  void print();

private:
  std::map<int64_t, std::weak_ptr<Entity>> _entities{};
};
