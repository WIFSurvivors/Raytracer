#pragma once

#include <map>
#include <memory>

#include "Entity.hpp"
#include "SimpleComponent.hpp"

// template<class T> requires
// struct system{
//     virtual bool add_component(Component c) = 0;
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

//     bool add_component(derived<Component> c);
//     bool remove_component(derived<Component> c);

// private:
//     std::vector<derived<Component>> _components;
// };

struct SimpleSystem //: system
{
  SimpleComponent *create_component(long e) { return nullptr; }
  SimpleComponent *create_component(long e, int value) { return nullptr; }
  SimpleComponent *create_component(std::shared_ptr<Entity> e);
  SimpleComponent *create_component(std::shared_ptr<Entity> e, int value);

  void clear();

  inline const auto &get_components() const { return _components; }
  // bool remove_component(SimpleComponent *c);

  void print_all_components();

  // void init() const;
  void update(const float dt);
  // void destroy() const;

private:
  std::map<long, std::unique_ptr<SimpleComponent>> _components{};
};