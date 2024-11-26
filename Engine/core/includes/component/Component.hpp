#pragma once

#include <boost/uuid/uuid.hpp>
#include <memory>
typedef boost::uuids::uuid uuid;

struct Entity; // forward declaration due to child-parent structure

struct Component {
  // Life cycle of a component:

  /// @brief Use the constructor to internally initialize the object
  Component(uuid id, Entity *e);

  /// @brief Appendum to consturctor for referencing external objects
  virtual void init();

  /// @brief Use to update component's logic each tick
  /// @param dt Time step
  virtual void update(float dt);

  /// @brief Use to delete references to external objects
  virtual void destroy();

  /// @brief Use the constructor to internally deconstruct the object
  virtual ~Component() = default;

  bool operator<(const Component &right) const { return _uuid < right._uuid; }

  uuid get_uuid();
  virtual void print() {}

  inline Entity *get_entity() { return _entity; }

protected:
  uuid _uuid;
  Entity *_entity;
};
