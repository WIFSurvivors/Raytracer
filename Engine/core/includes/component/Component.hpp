#pragma once

#include <boost/uuid/uuid.hpp>
typedef boost::uuids::uuid uuid;

struct Entity; // forward declaration due to child-parent structure

struct IComponent {
  /// @brief Create new component with an uuid and link it to an entity.
  IComponent(uuid id, Entity *e);

  /// @brief Destroy component. This will also unlink it from it's entity.
  /// Override this if your need to clean-up more resources.
  virtual ~IComponent() = default;

  /// @brief Use to update component's logic each tick
  /// @param dt Time step
  virtual void update(const float dt);

  virtual ~IComponent();

  // required by std::map<uuid, component>
  bool operator<(const IComponent &right) const { return _uuid < right._uuid; }

  inline uuid get_uuid() { return _uuid; }
  inline Entity *get_entity() { return _entity; }

protected:
  uuid _uuid;
  Entity *_entity; // Doesn't have Ownership!
};
