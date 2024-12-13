#pragma once
#include "includes/component/Component.hpp"
#include "includes/system/EntityStorage.hpp"
#include <boost/uuid/uuid.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>
#include <optional>

// typedef boost::uuids::uuid uuid;

struct Entity : public std::enable_shared_from_this<Entity> {
  using uuid = boost::uuids::uuid;
  // make private -> will require some friend magic...
  // Entity(Private);
  Entity();
  Entity(const std::string &name, uuid id);
  Entity(const std::string &name, uuid id, std::shared_ptr<Entity> parent);

  inline std::shared_ptr<Entity> get_ptr() { return shared_from_this(); }

  std::optional<IComponent *> get_component(uuid id);

  void add_component(IComponent *c);
  bool remove_component(IComponent *c);
  bool remove_component(uuid id);

  void add_child_entity(std::shared_ptr<Entity> e);
  std::vector<std::shared_ptr<Entity>> &get_child_entities();
  bool remove_child_entity(std::shared_ptr<Entity> e);

  inline std::weak_ptr<Entity> get_parent_entity() { return _parent; }

  inline glm::vec3 get_local_position() const { return _position; }
  inline void set_local_position(const glm::vec3 pos) { _position = pos; }
  glm::vec3 get_world_position() const;

  inline glm::vec3 get_local_rotation() const { return _rotation; }
  inline void set_local_rotation(const glm::vec3 rot) { _rotation = rot; }
  glm::vec3 get_world_rotation() const;

  inline glm::vec3 get_local_scale() const { return _scale; }
  inline void set_local_scale(const glm::vec3 scale) { _scale = scale; }
  glm::vec3 get_world_scale() const;

  inline uuid get_uuid() { return _uuid; }
  void print();

  inline const std::string &get_name() { return _name; }
  inline void set_name(const std::string &name) { _name = name; }
  inline const auto &get_components() const { return _components; }

private:
  friend class EntityStorage;
  //   friend std::shared_ptr<Entity>
  //   EntityStorage::create_root_entity(const std::string &name, uuid id);

  friend class std::shared_ptr<Entity>;
  //   EntityStorage::create_entity(const std::string &name, uuid id,
  //                                std::shared_ptr<Entity> parent);

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
  std::vector<IComponent *> _components{};

  void print(int indent);
};
