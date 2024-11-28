#pragma once
#include "glm/vec3.hpp"
#include "includes/component/Component.hpp"
#include "includes/system/EntitySystem.hpp"
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <string>
#include <vector>
#include <optional>

typedef boost::uuids::uuid uuid;

struct Entity : public std::enable_shared_from_this<Entity> {
  // make private -> will require some friend magic...
  // Entity(Private);
  Entity();
  Entity(const std::string &name, uuid id);
  Entity(const std::string &name, uuid id, std::shared_ptr<Entity> parent);

  inline std::shared_ptr<Entity> get_ptr() { return shared_from_this(); }

  std::optional<Component *> get_component(uuid id);

  void add_component(Component *c);
  bool remove_component(Component *c);
  bool remove_component(uuid id);

  void add_child_entity(std::shared_ptr<Entity> e);
  std::vector<std::shared_ptr<Entity>> &get_child_entities();
  bool remove_child_entity(std::shared_ptr<Entity> e);

  std::weak_ptr<Entity> get_parent_entity();

  glm::vec3 get_local_position() const;
  void set_local_position(const glm::vec3 pos);
  glm::vec3 get_world_position() const;

  glm::vec3 get_local_rotation() const;
  void set_local_rotation(const glm::vec3 pos);
  glm::vec3 get_world_rotation() const;

  glm::vec3 get_local_scale() const;
  void set_local_scale(const glm::vec3 pos);
  glm::vec3 get_world_scale() const;

  inline uuid get_uuid() { return _uuid; }
  void print();

  const std::string &get_name();
  void set_name(const std::string &name);

private:
  friend std::shared_ptr<Entity>
  EntitySystem::create_root(const std::string &name, uuid id);

  friend std::shared_ptr<Entity>
  EntitySystem::create_entity(const std::string &name, uuid id,
                              std::shared_ptr<Entity> parent);

  static std::shared_ptr<Entity> create(const std::string &name, uuid id,
                                        std::shared_ptr<Entity> parent);
  static std::shared_ptr<Entity> create(const std::string &name, uuid id);

  uuid _uuid{};
  std::string _name{"untitled"};

  glm::vec3 _position{};
  glm::vec3 _rotation{};
  glm::vec3 _scale{1.f, 1.f, 1.f};

  std::weak_ptr<Entity> _parent{};
  std::vector<std::shared_ptr<Entity>> _child_entities{};
  std::vector<Component *> _components{};

  void print(int indent);
  // int _parent_uuid{};
  // std::vector<long> _child_entities_uuids{};
  // std::vector<long> _component_uuids{};
};
