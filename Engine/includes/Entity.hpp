#pragma once
#include "includes/component/Component.hpp"
#include "includes/system/EntityStorage.hpp"
#include "includes/utility/JSONConvertable.hpp"
#include <boost/uuid/uuid.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <vector>
#include <optional>

// typedef boost::uuids::uuid uuid;
namespace RT {
struct Entity : public std::enable_shared_from_this<Entity>,
                public JSONConvertable {
  using uuid = boost::uuids::uuid;

  inline std::shared_ptr<Entity> get_ptr() { return shared_from_this(); }

  std::optional<IComponent *> get_component(uuid id) const;

  void add_component(IComponent *c);
  bool remove_component(IComponent *c);
  bool remove_component(uuid id);
  inline bool has_Updated(){ return _updated;}
  inline void did_update(){_updated=false;}

  void add_child_entity(std::shared_ptr<Entity> e);
  bool remove_child_entity(std::shared_ptr<Entity> e);
  inline const std::vector<std::shared_ptr<Entity>> &get_child_entities() const {
    return _child_entities;
  }

  inline std::weak_ptr<Entity> get_parent_entity() const { return _parent; }

  inline glm::vec3 get_local_position() const { return _position; }
  inline void set_local_position(float x, float y, float z) {
    _position.x = x;
    _position.y = y;
    _position.z = z;
     _updated = true;
  }
  inline void set_local_position(const glm::vec3 pos) { _position = pos;  _updated = true;}
  glm::vec3 get_world_position() const;

  inline glm::vec3 get_local_rotation() const { return _rotation; }
  inline void set_local_rotation(const glm::vec3 rot) { _rotation = rot;  _updated = true; }
  inline void set_local_rotation(float x, float y, float z) {
    _rotation.x = x;
    _rotation.y = y;
    _rotation.z = z;
     _updated = true;
  }
  glm::vec3 get_world_rotation() const;

  inline glm::vec3 get_local_scale() const { return _scale; }
  inline void set_local_scale(const glm::vec3 scale) { _scale = scale;  _updated = true;}
  inline void set_local_scale(float x, float y, float z) {
    _scale.x = x;
    _scale.y = y;
    _scale.z = z;
     _updated = true;
  }
  glm::vec3 get_world_scale() const;

  inline uuid get_uuid() const { return _uuid; }

  inline const std::string &get_name() const { return _name; }
  inline void set_name(const std::string &name) { _name = name; }
  inline auto get_components() { return _components; }

  boost::json::object to_json() const override;
  void print();

private:
  // EntityStorage is the only class that can call create(...) !!!!
  friend class EntityStorage;
  static std::shared_ptr<Entity> create(const std::string &name, uuid id,
                                        std::shared_ptr<Entity> parent);
  static std::shared_ptr<Entity> create(const std::string &name, uuid id);

  // required for smart poiner to create/destroy entities, see:
  // https://stackoverflow.com/questions/8147027/how-do-i-call-stdmake-shared-on-a-class-with-only-protected-or-private-const
  struct make_shared_enabler;
  Entity(const std::string &name, uuid id);
  Entity(const std::string &name, uuid id, std::shared_ptr<Entity> parent);

  boost::json::object self_to_json() const;
  boost::json::array children_to_json() const;

  uuid _uuid{};
  std::string _name{"untitled"};

  glm::vec3 _position{};
  glm::vec3 _rotation{};
  glm::vec3 _scale{1.f, 1.f, 1.f};


  bool _updated = true;
  
  std::weak_ptr<Entity> _parent{};
  std::vector<std::shared_ptr<Entity>> _child_entities{};
  std::vector<IComponent *> _components{};
};

struct Entity::make_shared_enabler : public Entity {
  make_shared_enabler(const std::string &name, uuid id) : Entity(name, id) {}
  make_shared_enabler(const std::string &name, uuid id,
                      std::shared_ptr<Entity> parent)
      : Entity(name, id, parent) {}
};
} // namespace RT
