#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>
#include <optional>
#include <memory>
#include <map>

typedef boost::uuids::uuid uuid;

struct Entity;
struct Component;

/*
A System handles UUID to Component or Entity bindings and can provide
system-wide attributes and methods.
*/
// template <class T> // components :)
struct System {
  System() = default;
  virtual ~System() = default;

  /**
   * A component is always linked to an entity. It is recommended to overwrite
   * return type suited for the system.
   */
  virtual Component *create_component(uuid id, Entity *e);

  /**
   * Entity arestd::vector<glm::vec3>& vertices, std::vector<glm::vec2>& UV
   * responsible for representing components in 3D space.
   */
  virtual std::shared_ptr<Entity> create_entity(const std::string &name,
                                                uuid id);

  /**
   * Get Component stored in this system. Will return std::nullopt when UUID is
   * not found.
   */
  virtual std::optional<Component *> get_component(uuid id);

  /**
   * Get Entity stored in this system. Will return std::nullopt when UUID is
   * not found.
   */
  virtual std::optional<Entity *> get_entity(uuid id);

  /**
   * When removing a component, it also needs to be removed from it's designated
   * entity. Each component knows it's entity. (easy)
   */
  virtual bool remove(Component *c);

  /**
   * When removing an entity, we also need to remove all it's child entites
   * (easy) as well as all it's components. The latter is more difficult,
   * because components can be part of different system.
   */
  virtual bool remove(Entity *e);

  /**
   * Depending on the uuid, you are either removing an entity or component. A
   * system usually only supports one of those types and should return false or
   * error when uuid is not found.
   */
  virtual bool remove(uuid uuid) = 0;

  virtual void clear() = 0;

  virtual void print();

protected:
//   std::map<uuid, T> _components; // HOW TO DO THIS FOR ENTITY?!?!?!?
};
