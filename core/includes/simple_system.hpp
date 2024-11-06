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
  simple_component *create_component(std::shared_ptr<entity> e);
  simple_component *create_component(std::shared_ptr<entity> e, int value);

  ~simple_system();

  void clear();
  // bool remove_component(simple_component *c);

  inline const auto &get_components() { return _components; }

  void print_all_components();

  // const correctness might be logically wrong here?
  // need to double check const again
  // void init() const;
  void update(const float dt) const;
  // void destroy() const;

private:
  // <uuid, component>
  std::map<long, std::unique_ptr<simple_component>> _components{};
};