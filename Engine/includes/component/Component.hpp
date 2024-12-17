#pragma once

#include "includes/utility/JSONConvertable.hpp"
#include <boost/uuid/uuid.hpp>
typedef boost::uuids::uuid uuid;

struct Entity; // forward declaration due to child-parent structure

struct IComponent : public JSONConvertable {
  /// @brief Create new component with an uuid and link it to an entity.
  IComponent(uuid id, Entity *e);

  /// @brief Destroy component. This will also unlink it from it's entity.
  /// Override this if your need to clean-up more resources.
  virtual ~IComponent();

  /// @brief Use to update component's logic each tick
  /// @param dt Time step
  virtual void update(const float dt) = 0;

  // required by std::map<uuid, component>
  bool operator<(const IComponent &right) const { return _uuid < right._uuid; }

  inline uuid get_uuid() { return _uuid; }
  inline Entity *get_entity() { return _entity; }

  boost::json::object to_json() override;
  
protected:
  uuid _uuid;
  Entity *_entity; // Doesn't have Ownership!
  
  boost::json::object to_json_base();
  virtual void to_json_details(boost::json::object& obj) = 0;
};