#pragma once

#include <memory>
#include <set>

#include "entity.hpp"
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

struct simple_system //: system
{
  simple_component *create_component(entity &e);
  simple_component *create_component(entity &e, int value);

  // bool remove_component(simple_component *c);

  inline const auto &get_components() { return _components; }

  void print_all_components();

private:
  std::set<std::unique_ptr<simple_component>>
      _components{}; // make set instead?
};