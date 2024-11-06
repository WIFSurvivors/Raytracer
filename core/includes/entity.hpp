#pragma once
#include "component.hpp"
#include "glm/vec3.hpp"
#include "scene.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

struct entity : public std::enable_shared_from_this<entity> {

  // make private -> will require some friend magic...
  // entity(Private);
  entity();
  entity(std::string name, int64_t uuid);
  entity(std::string name, int64_t uuid, std::shared_ptr<entity> parent);

  inline std::shared_ptr<entity> get_ptr() { return shared_from_this(); }

  bool get_component(int64_t uuid, component &c);

  void add_component(component *c);
  void remove_component(component *c);
  void remove_component(int64_t uuid);

  void add_child_entity(std::shared_ptr<entity> e);

  std::weak_ptr<entity> get_parent_entity();

  glm::vec3 position{};
  glm::vec3 rotation{};
  glm::vec3 scale{1.f, 1.f, 1.f};

  inline int64_t get_uuid() { return _uuid; }
  void print();

private:
  // friend std::shared_ptr<entity>;
  // friend std::shared_ptr<entity> std::make_shared<entity>();
  // only scene is allowed to create entities... using create...
  friend std::shared_ptr<entity>
  scene::create_entity(std::string name, int64_t uuid,
                       std::shared_ptr<entity> parent);

  static std::shared_ptr<entity> create(std::string name, int64_t uuid,
                                        std::shared_ptr<entity> parent);

  static std::shared_ptr<entity> create(std::string name, int64_t uuid);

  int64_t _uuid{-1};
  std::string _name{"untitled"};

  std::weak_ptr<entity> _parent{};
  std::vector<std::shared_ptr<entity>> _child_entities{};
  std::vector<component *> _components{};

  void print(int indent);

  // TODO: Add method to
  // int _parent_uuid{};
  // std::vector<long> _child_entities_uuids{};
  // std::vector<long> _component_uuids{};
};
