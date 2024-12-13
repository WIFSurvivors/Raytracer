#pragma once

#include "includes/component/Component.hpp"
#include "includes/utility/SimpleLogger.hpp"
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

// typedef boost::uuids::uuid uuid;
struct Entity;

template <typename T>
concept is_base_of_component = std::is_base_of<IComponent, T>::value;

/**
 * Parameterless System class, that can be used whenever you don't care about
 * the underlying type T of System. Imagine this is a sort of placeholder!
 */
class ISystem {
public:
  virtual ~ISystem() = default;

  inline virtual std::string get_system_name() const = 0;
};

/*
 * A System creates Components by binding them to UUID and Entity. Additionally
 * it can provide system-wide attributes and methods.
 */
template <is_base_of_component T> struct System : public ISystem {
  using uuid = boost::uuids::uuid;

  System() = default;
  virtual ~System() override = default;

  /**
   * Get Component stored in this system. Will return std::nullopt when UUID is
   * not found.
   */
  std::optional<T *> get_component(uuid id) {
    if (!_components.contains(id)) {
      SimpleLogger::print(std::format("-- ! component {} not found",
                                      boost::uuids::to_string(id)));
      return {};
    }
    return std::make_optional(_components[id].get());
  }

  /**
   * Removes Component from container by component pointer.
   * This will call remove(uuid)
   */
  bool remove(T *c) { return remove(c->get_uuid()); }

  /**
   * Removes Component from container by uuid.
   * This will also remove it's link to it's entity.
   */
  bool remove(uuid id) {
    // Because each component is a unique_ptr, it will call deconstructor of
    // IComponent on destruction, which in turn will remove itself from it's
    // linked entity
    SimpleLogger::print(std::format("-- ! removing component with UUID {}",
                                    boost::uuids::to_string(id)));
    return _components.erase(id);
  }

  /**
   * Removes all components from container. Assumption is, that each component's
   * deconstructor handles deletion properly!
   */
  void clear() {
    SimpleLogger::print("-- !! clearing all components from system");
    _components.clear();
  }

  /**
   * Prints all components of the system
   */
  virtual void print() = 0;
//   {
// 	VariadicTable<std::string, int> vt({"UUID", "sample :3"}, 10);

//     std::for_each(_components.begin(), _components.end(), [this, &vt](auto &n) mutable {
// 	  vt.addRow(boost::uuids::to_string(n.first), 123);
//     //   print_component(*(n.second.get())); 
//       std::cout << "\n";
//     });

// 	// vt.addRow("Cody", 180.2, 40, "John");
// 	// vt.addRow("David", 175.3, 38, "Andrew");
// 	// vt.addRow("Robert", 140.3, 27, "Fande");

// 	vt.print(std::cout);
//   }

protected:
  /**
   * A component is always linked to an entity.
   * This method is not public, because the user should implement their
   * own method to allow for extra parameters. It is advised to then call this
   * function as a first step!
   */
  T *create_component_base(uuid id, Entity *e) {
    SimpleLogger::print("-- create component");
    _components[id] = std::make_unique<T>(id, e);
    auto ptr = _components[id].get();
    // e->add_component(ptr); // this is handled in IComponent Constructor!!
    return ptr; // pointer can be used by child classes for further
                // configuration
  }

  std::map<uuid, std::unique_ptr<T>> _components;
};
