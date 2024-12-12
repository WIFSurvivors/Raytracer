#include "includes/system/System.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include <boost/uuid/uuid_io.hpp>
// #include <string>
#include <algorithm>

template <is_base_of_component T>
T *ISystem<T>::create_component(uuid id, Entity *e) {
  SimpleLogger::print("-- create component");
  _components[id] = std::make_unique<T>(id, e);
  auto ptr = _components[id].get();
  // e->add_component(ptr); // this is handled in IComponent Constructor!!
  return ptr; // pointer can be used by child classes for further configuration
}

template <is_base_of_component T>
std::optional<T *> ISystem<T>::get_component(uuid id) {
  return _components[id];
}

template <is_base_of_component T> bool ISystem<T>::remove(T *c) {
  return remove(c->get_uuid());
}

template <is_base_of_component T> bool ISystem<T>::remove(uuid id) {
  // Because each component is a unique_ptr, it will call deconstructor of
  // IComponent on destruction, which in turn will remove itself from it's linked
  // entity
  SimpleLogger::print(std::format("-- ! removing component with UUID {}",
                                  boost::uuids::to_string(id)));
  return _components.erase(id);
}

template <is_base_of_component T> void ISystem<T>::clear() {
  SimpleLogger::print("-- !! clearing all components from system");
  _components.clear();
}

template <is_base_of_component T> void ISystem<T>::print() {
  // probably ues ostringstream for this
  // or just std::string? (i do like std::format uwu)
  std::for_each(_components.begin(), _components.end(), [this](auto &n) {
    std::cout << boost::uuids::to_string(n.first) << " : ";
    print_component(*(n.second.get())); // make print return std::string ...
    std::cout << "\n";
  });
}
