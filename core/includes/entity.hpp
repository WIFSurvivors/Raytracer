#pragma once
#include "component.hpp"
#include "glm/vec3.hpp"
#include "scene.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

struct entity {
  entity(std::string name, std::weak_ptr<entity> parent, long uuid = -1);

  bool get_component(long uuid, component &c);

  void add_component(component *c);
  void remove_component(component *c);
  void remove_component(long uuid);

  glm::vec3 position{};
  glm::vec3 rotation{};
  glm::vec3 scale{1.f, 1.f, 1.f};

  inline long get_uuid() { return _uuid; }
  void print();

private:
  friend scene::scene();
  friend std::unique_ptr<entity> std::make_unique<entity>();

  entity(); // called by scene as friend :3
  long _uuid{-1};
  std::string _name{"untitled"};

  std::weak_ptr<entity> _parent{};
  std::vector<std::shared_ptr<entity>> _child_entities{};
  std::vector<component *> _components{};

  // int _parent_uuid{};
  // std::vector<long> _child_entities_uuids{};
  // std::vector<long> _component_uuids{};
};
