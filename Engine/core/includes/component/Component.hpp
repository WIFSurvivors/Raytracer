#pragma once

#include <boost/uuid/uuid.hpp>
typedef boost::uuids::uuid uuid;

struct Entity; // forward declaration due to child-parent structure

struct Component {
  /// @brief Use the constructor to internally initialize the object
  Component(uuid id, Entity *e);

  /// @brief Appendum to consturctor for referencing external objects
  virtual void init();

  /// @brief Use to update component's logic each tick
  /// @param dt Time step
  virtual void update(const float dt);

  /// @brief Use to delete references to external objects
  virtual void destroy();

  /// @brief Use the constructor to internally deconstruct the object
  virtual ~Component();

  // required by std::map<uuid, component>
  bool operator<(const Component &right) const { return _uuid < right._uuid; }

  inline uuid get_uuid() {return _uuid;}
  inline Entity *get_entity() { return _entity; }

protected:
  uuid _uuid;
  Entity *_entity;
};
