#pragma once

#include "includes/UUIDManager.hpp"
#include "includes/component/Component.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/VariadicTable.hpp"
#include "includes/Entity.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <algorithm>
#include <memory>
#include <optional>
#include <memory>
#include <map>
#include <type_traits>
#include <map>
#include <format>
#include <string>

namespace RT {
struct Entity;

template <typename T>
concept is_base_of_component = std::is_base_of<IComponent, T>::value;

/*
 * A System creates Components by binding them to UUID and Entity. Additionally
 * it can provide system-wide attributes and methods.
 */
template <is_base_of_component T> struct System : public IStorage {
  using uuid = boost::uuids::uuid;

  explicit System(UUIDManager *um)
      : IStorage(um) { /*LOG(std::format("created {}", get_name()));*/ }
  ~System() override = default;

  /**
   * Get Component stored in this system. Will return std::nullopt when UUID is
   * not found.
   */
  inline std::optional<T *> get_component(uuid id) {
    if (!_components.contains(id)) {
      LOG_WARN(std::format("-- ! component {} not found",
                           boost::uuids::to_string(id)));
      return {};
    }
    return std::make_optional(_components[id].get());
  }

  /**
   * Removes Component from container by component pointer.
   * This will call remove(uuid)
   */
  inline bool remove(T *c) { return remove(c->get_uuid()); }

  /**
   * Removes Component from container by uuid.
   * This will also remove it's link to it's entity.
   */
  inline bool remove(uuid id) {
    // Because each component is a unique_ptr, it will call deconstructor of
    // IComponent on destruction, which in turn will remove itself from it's
    // linked entity
    LOG(std::format("-- ! removing component with UUID {}",
                    boost::uuids::to_string(id)));
    return _components.erase(id);
  }

  /**
   * Removes all components from container. Assumption is, that each component's
   * deconstructor handles deletion properly!
   */
  void clear() {
    LOG("-- !! clearing all components from system");
    _components.clear();
  }

  inline const std::map<uuid, std::unique_ptr<T>> &get_components() {
    return _components;
  }

  /**
   * Prints all components of the system
   */
  virtual void print() = 0;

protected:
  /**
   * A component is always linked to an entity.
   * This method is not public, because the user should implement their
   * own method to allow for extra parameters. It is advised to then call this
   * function as a first step!
   */
  T *create_component_base(Entity *e, uuid id) {
    if (!_um->add(id, this)) {
      LOG_WARN(std::format(
          "Component ({}) has been added to Entity {} ({}), but this bring "
          "will inconsitent behaviour when accessing via UUID!",
          boost::uuids::to_string(id), e->get_name(),
          boost::uuids::to_string(e->get_uuid())));
      return nullptr;
    }
    _components[id] = std::make_unique<T>(id, e);
    auto ptr = _components[id].get();
    return ptr;
  }

  /**
   * A component is always linked to an entity.
   * This method is not public, because the user should implement their
   * own method to allow for extra parameters. It is advised to then call this
   * function as a first step!
   */
  T *create_component_base(Entity *e) {
    auto id = _um->create(this);
    _components[id] = std::make_unique<T>(id, e);
    auto ptr = _components[id].get();
    return ptr;
  }

  std::map<uuid, std::unique_ptr<T>> _components;
};
} // namespace RT
