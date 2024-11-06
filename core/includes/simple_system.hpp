#pragma once

#include <map>
#include <memory>

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
  simple_component *create_component(long e) { return nullptr; }
  simple_component *create_component(long e, int value) { return nullptr; }
  simple_component *create_component(std::shared_ptr<entity> e);
  simple_component *create_component(std::shared_ptr<entity> e, int value);

  void clear();

  inline const auto &get_components() const { return _components; }
  // bool remove_component(simple_component *c);

  void print_all_components();

  // void init() const;
  void update(const float dt);
  // void destroy() const;

private:
  std::map<long, std::unique_ptr<simple_component>> _components{};
};