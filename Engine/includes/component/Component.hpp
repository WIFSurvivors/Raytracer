#pragma once

#include "includes/utility/JSONConvertable.hpp"
#include <boost/uuid/uuid.hpp>
#include <string>
typedef boost::uuids::uuid uuid;

struct Entity; // forward declaration due to child-parent structure
struct Timer;

struct IComponent : public JSONConvertable {
  /// @brief Create new component with an uuid and link it to an entity.
  IComponent(uuid id, Entity *e, std::string name);

  /// @brief Destroy component. This will also unlink it from it's entity.
  /// Override this if your need to clean-up more resources.
  virtual ~IComponent();

  /// @brief Use to update component's logic each tick
  /// @param dt Time step
  virtual void update(const Timer& timer) = 0;

  // required by std::map<uuid, component>
  bool operator<(const IComponent &right) const { return _uuid < right._uuid; }

  inline uuid get_uuid() { return _uuid; }
  inline Entity *get_entity() { return _entity; }

  inline const std::string &get_component_name() const {
    return _component_name;
  }
  boost::json::object to_json() override;
  boost::json::object to_json_short();
  virtual boost::json::object to_json_details() = 0;
  virtual void set_from_json(boost::json::object obj) = 0;

protected:
  std::string _component_name;
  uuid _uuid;
  Entity *_entity; // Doesn't have Ownership!

  boost::json::object to_json_base();
  virtual void to_json_details(boost::json::object obj) = 0;
};
