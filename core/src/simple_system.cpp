#include "simple_system.hpp"

template <class T> simple_system<T>::simple_system() {}

template <class T>
bool simple_system<T>::add_component(simple_component<T> &c) {
  return _components.insert(c);
}

template <class T>
bool simple_system<T>::remove_component(simple_component<T> &c) {
  return _components.erase(c);
}
