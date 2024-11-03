#pragma once

#include <memory>
// #include "entity.hpp"
struct entity;

static long component_count = 1000;

struct component {
  // Life cycle of a component:

  /// @brief Use the constructor to internally initialize the object
  component(std::weak_ptr<entity> e);

  /// @brief Appendum to consturctor for referencing external objects
  virtual void init();

  /// @brief Use to update component's logic each tick
  /// @param dt Time step
  virtual void update(float dt);

  /// @brief Use to delete references to external objects
  virtual void destroy();

  /// @brief Use the constructor to internally deconstruct the object
  virtual ~component() = default;

  bool operator<(const component &right) const { return _uuid < right._uuid; }

  long get_uuid();
  virtual void print() = 0;

protected:
  long _uuid;
  std::weak_ptr<entity> _entity;
};
