#pragma once

#include "includes/Component.hpp"
#include "includes/Entity.hpp"
#include <cstdint>

struct System {
  virtual ~System() = default;
  virtual Component *create_component(int64_t uuid) {}
  virtual Entity *create_entity(int64_t uuid) {}
  virtual bool remove(Component *c) {}
  virtual bool remove(Entity *e) {}
  virtual bool remove(int64_t uuid) = 0;

private:
};
