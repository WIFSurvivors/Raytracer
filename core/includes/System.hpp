#pragma once

#include "includes/Component.hpp"
#include "includes/Entity.hpp"
#include <cstdint>
#include <string>

/*
A System handles UUID-Component data and provides some system-wide attributes
and methods.
*/
struct System {
  virtual ~System() = default;
  virtual std::weak_ptr<Component> create_component(int64_t uuid);
  virtual std::shared_ptr<Entity> create_entity(const std::string &name,
                                                int64_t uuid);
  virtual bool remove(Component *c);
  virtual bool remove(Entity *e);
  virtual bool remove(int64_t uuid) = 0;
};
