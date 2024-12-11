#pragma once

#include <boost/uuid/uuid.hpp>
typedef boost::uuids::uuid uuid;

struct Entity; // forward declaration due to child-parent structure

struct Component {
  /// @brief Create new component with an uuid and link it to an entity.
  Component(uuid id, Entity *e);

  /// @brief Destroy component. This will also unlink it from it's entity.
  /// Override this if your need to clean-up more resources.
  virtual ~Component() = default;

  /// @brief Use to update component's logic each tick
  /// @param dt Time step
  virtual void update(const float dt);

  virtual ~Component();

  // required by std::map<uuid, component>
  bool operator<(const Component &right) const { return _uuid < right._uuid; }

  inline uuid get_uuid() { return _uuid; }
  inline Entity *get_entity() { return _entity; }

protected:
  uuid _uuid;
  Entity *_entity; // Doesn't have Ownership!
};
