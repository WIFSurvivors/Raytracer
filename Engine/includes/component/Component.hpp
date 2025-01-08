#pragma once

#include "includes/utility/JSONConvertable.hpp"
#include <boost/uuid/uuid.hpp>
#include <string>

namespace RT {
struct Entity; // forward declaration due to child-parent structure
struct FrameSnapshot;

struct IComponent : public JSONConvertable {
  using uuid = boost::uuids::uuid;

  /// @brief Create new component with an uuid and link it to an entity.
  IComponent(uuid id, Entity *e, std::string name);

  /// @brief Destroy component. This will also unlink it from it's entity.
  /// Override this if your need to clean-up more resources.
  virtual ~IComponent();

  /// @brief Use to update component's logic with total time of the programm
  virtual void update(const FrameSnapshot &snapshot) = 0;

  // required by std::map<uuid, component>
  bool operator<(const IComponent &right) const { return _uuid < right._uuid; }

  inline const uuid get_uuid() const { return _uuid; }
  inline Entity *get_entity() { return _entity; }
  inline Entity *get_entity() const { return _entity; }
  inline const std::string &get_component_name() const {
    return _component_name;
  }

  boost::json::object to_json() const override;
  boost::json::object to_json_short() const;

  virtual void set_from_json(boost::json::object obj) = 0;

protected:
  std::string _component_name;
  uuid _uuid;
  Entity *_entity; // Doesn't have Ownership!

  boost::json::object to_json_base() const;
  virtual boost::json::object to_json_details() const = 0;
};
} // namespace RT
