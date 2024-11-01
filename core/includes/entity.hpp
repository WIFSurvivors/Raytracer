#pragma once
#include "component.hpp"
#include <map>
#include <memory>
#include <string>

struct entity {
  std::vector<component *> &get_components();
  void add_component(component *c);
  void remove_component(component *c);
  void remove_component(long uuid);

private:
  entity();
  long _uuid{-1};
  std::string _name{"untitled"};

  std::weak_ptr<entity> _parent{};
  int _parent_uuid{};
  std::vector<std::shared_ptr<entity>> _child_entities{};
  std::vector<long> _child_entities_uuids{};

  std::vector<long> _component_uuids{};
  std::vector<component *> _components{};
};
