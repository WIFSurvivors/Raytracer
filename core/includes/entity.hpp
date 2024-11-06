#pragma once
#include "component.hpp"
#include "glm/vec3.hpp"
#include "scene.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

struct entity : public std::enable_shared_from_this<entity> {
private:
  // struct Private {
  //   explicit Private() = default;
  // };

public:
  // entity(std::string name, std::shared_ptr<entity> parent, long uuid = -1);
  virtual ~entity();

  inline std::shared_ptr<entity> get_ptr() { return shared_from_this(); }

  bool get_component(long uuid, component &c);

  void add_component(component *c);
  void remove_component(component *c);
  void remove_component(long uuid);

  void add_child_entity(std::shared_ptr<entity> e);

  std::weak_ptr<entity> get_parent_entity();

  glm::vec3 position{};
  glm::vec3 rotation{};
  glm::vec3 scale{1.f, 1.f, 1.f};

  inline long get_uuid() { return _uuid; }
  void print();

  // entity(Private);
  // only scene is allowed to create entities... using create...
  entity();
  entity(std::string name, long uuid);
  entity(std::string name, long uuid, std::shared_ptr<entity> parent);

private:
  // friend std::shared_ptr<entity>;
  // friend std::shared_ptr<entity> std::make_shared<entity>();
  friend std::shared_ptr<entity>
  scene::create_entity(std::string name, std::shared_ptr<entity> parent,
                       long uuid);

  static std::shared_ptr<entity> create(std::string name, long uuid,
                                        std::shared_ptr<entity> parent);
  static std::shared_ptr<entity>
  create(std::string name, long uuid); // called by scene as friend :3

  long _uuid{-1};
  std::string _name{"untitled"};

  std::weak_ptr<entity> _parent{};
  std::vector<std::shared_ptr<entity>> _child_entities{};
  std::vector<component *> _components{};

  void print(int indent);

  // int _parent_uuid{};
  // std::vector<long> _child_entities_uuids{};
  // std::vector<long> _component_uuids{};
};
