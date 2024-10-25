#pragma once

#include <concepts>
#include <set>

#include "component.hpp"
#include "simple_component.hpp"

// template<class T> requires
// struct system{
//     virtual bool add_component(component c) = 0;
// };

// template<class T, class U>
// concept derived = std::is_base_of<U, T>::value;

// namespace concepts_example {
//     struct a{};
//     struct b:a{};
//     struct c{};

//     void func(){
//         derived<a> auto t = a{};
//         derived<a> auto t = b{};
//         // derived<a> auto t = c{}; // error because c is not derived from a
//     }
// }

// template <class T>
// struct system{

//     bool add_component(derived<component> c);
//     bool remove_component(derived<component> c);

// private:
//     std::vector<derived<component>> _components;
// };

template <class T>
struct simple_system //: system
{
  simple_system();

  bool add_component(simple_component<T> &c);
  bool remove_component(simple_component<T> &c);

  inline const std::set<simple_component<T>> &get_components() {
    return _components;
  }

private:
  // std::set<derived<component>> _components;
  std::set<simple_component<T>> _components{};
};
