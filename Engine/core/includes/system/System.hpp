#pragma once

#include "includes/component/Component.hpp"
#include "includes/Entity.hpp"

#include <boost/uuid/uuid.hpp>
#include <memory>
#include <optional>
#include <memory>
#include <map>
#include <type_traits>
#include <map>

// typedef boost::uuids::uuid uuid;
struct Entity;

template <typename T>
concept is_base_of_component = std::is_base_of<Component, T>::value;

/*
 * A System creates Components by binding them to UUID and Entity. Additionally
 * it can provide system-wide attributes and methods.
 */
template <is_base_of_component T> struct ISystem {
  using uuid = boost::uuids::uuid;

  ISystem() = default;
  virtual ~ISystem() = default;

  /**
   * Get Component stored in this system. Will return std::nullopt when UUID is
   * not found.
   */
  std::optional<T *> get_component(uuid id);

  /**
   * Removes Component from container by component pointer.
   * This will call remove(uuid)
   */
  bool remove(T *c);

  /**
   * Removes Component from container by uuid.
   * This will also remove it's link to it's entity.
   */
  bool remove(uuid uuid);

  /**
   * Removes all components from container. Assumption is, that each component's
   * deconstructor handles deletion properly!
   */
  void clear();

  /**
   * Prints all components of the system
   */
  void print();

protected:
  /**
   * A component is always linked to an entity.
   * This method is not public, because the user should implement their
   * own method to allow for extra parameters. It is advised to then call this
   * function as a first step!
   */
  T *create_component(uuid id, Entity *e);

  virtual void print_component(const T &c) = 0;

  std::map<uuid, std::unique_ptr<T>> _components;
};
