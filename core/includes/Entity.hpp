#pragma once
#include "glm/vec3.hpp"
#include "includes/Component.hpp"
#include "includes/EntitySystem.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <optional>

struct Entity : public std::enable_shared_from_this<Entity> {
  // make private -> will require some friend magic...
  // Entity(Private);
  Entity();
  Entity(std::string name, int64_t uuid);
  Entity(std::string name, int64_t uuid, std::shared_ptr<Entity> parent);

  inline std::shared_ptr<Entity> get_ptr() { return shared_from_this(); }

  std::optional<Component *> get_component(int64_t uuid);

  void add_component(Component *c);
  void remove_component(Component *c);
  void remove_component(int64_t uuid);

  void add_child_entity(std::shared_ptr<Entity> e);
  std::vector<std::shared_ptr<Entity>> &get_child_entities();

  std::weak_ptr<Entity> get_parent_entity();

  glm::vec3 position{};
  glm::vec3 rotation{};
  glm::vec3 scale{1.f, 1.f, 1.f};

  inline int64_t get_uuid() { return _uuid; }
  void print();

private:
  friend std::shared_ptr<Entity>
  EntitySystem::create_entity(std::string name, int64_t uuid,
                              std::shared_ptr<Entity> parent);

  static std::shared_ptr<Entity> create(std::string name, int64_t uuid,
                                        std::shared_ptr<Entity> parent);
  static std::shared_ptr<Entity> create(std::string name, int64_t uuid);

  int64_t _uuid{-1};
  std::string _name{"untitled"};

  std::weak_ptr<Entity> _parent{};
  std::vector<std::shared_ptr<Entity>> _child_entities{};
  std::vector<Component *> _components{};

  void print(int indent);
  // int _parent_uuid{};
  // std::vector<long> _child_entities_uuids{};
  // std::vector<long> _component_uuids{};
};
