#pragma once

#include <memory>
// #include "Entity.hpp"
struct Entity;

static long component_count = 1000;

struct Component {
  // Life cycle of a component:

  /// @brief Use the constructor to internally initialize the object
  Component(std::weak_ptr<Entity> e);

  /// @brief Appendum to consturctor for referencing external objects
  virtual void init();

  /// @brief Use to update component's logic each tick
  /// @param dt Time step
  virtual void update(float dt);

  /// @brief Use to delete references to external objects
  virtual void destroy();

  /// @brief Use the constructor to internally deconstruct the object
  // virtual ~component();

  bool operator<(const Component &right) const { return _uuid < right._uuid; }

  int64_t get_uuid();
  virtual void print() = 0;

  std::weak_ptr<Entity> _entity;

protected:
  long _uuid;
};
