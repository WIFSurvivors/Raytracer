#pragma once

#include <boost/uuid/uuid.hpp>
#include <memory>
typedef boost::uuids::uuid uuid;

struct Entity; // forward declaration due to child-parent structure

struct Component {
  // Life cycle of a component:

  /// @brief Use the constructor to internally initialize the object
  explicit Component(uuid id, std::weak_ptr<Entity> e);

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
  virtual void print() = 0;

  std::weak_ptr<Entity> _entity;

protected:
  uuid _uuid;
};
