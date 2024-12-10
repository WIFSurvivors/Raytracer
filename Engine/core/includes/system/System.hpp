#pragma once

#include "includes/component/Component.hpp"

#include <boost/uuid/uuid.hpp>
#include <string>
#include <memory>
#include <optional>
#include <memory>
#include <map>
#include <type_traits>
#include <map>
#include <algorithm>

typedef boost::uuids::uuid uuid;

struct Entity;

template <typename T>
concept is_base_of_component = std::is_base_of<Component, T>::value;

/*
A System handles UUID to Component or Entity bindings and can provide
system-wide attributes and methods.
*/
template <is_base_of_component T> struct ISystem {
  ISystem() = default;
  virtual ~ISystem() = default;

  /**
   * Get Component stored in this system. Will return std::nullopt when UUID is
   * not found.
   */
  virtual std::optional<T *> get_component(uuid id);

  /**
   * Removes Component from container by component pointer.
   * This will call remove(uuid)
   */
  bool remove(T *c);

  /**
   * Removes Component from container by uuid.
   * This will also remove it's link to it's entity.
   */
  virtual bool remove(uuid uuid);

  /**
   * Removes all components from container. This will call remove(uuid) on all
   * components.
   */
  void clear();

  /**
   * Prints all components of the system
   */
  void print();

protected:
  virtual void print_component() = 0;

  /**
   * A component is always linked to an entity. It is recommended to overwrite
   * return type suited for the system.
   * This method is not public, because the user is advised to implement their
   * own method to allow for extra parameters. It is advised to then call this
   * function as a first step!
   */
  virtual T *create_component(uuid id, Entity *e);

  std::map<uuid, std::unique_ptr<T>> _components;
};
