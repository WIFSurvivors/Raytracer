#pragma once

#include <memory>
#include <map>

struct Entity;

struct EntitySystem {
  EntitySystem();

  std::shared_ptr<Entity> create_entity(std::string name,
                                        std::shared_ptr<Entity> parent);
  std::shared_ptr<Entity> create_entity(std::string name, int64_t uuid,
                                        std::shared_ptr<Entity> parent);

  bool remove_entity(int64_t uuid);

  void print();

private:
  std::map<int64_t, std::weak_ptr<Entity>> _entities{};
};
