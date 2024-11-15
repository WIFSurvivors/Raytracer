#pragma once
#include "includes/Entity.hpp"
#include "includes/SimpleComponent.hpp"
#include "includes/System.hpp"
#include <map>
#include <memory>

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

struct SimpleSystem : public System {
  virtual Component *create_component(int64_t uuid) override;
  virtual bool remove(Component *c) override;

  SimpleComponent *create_component(std::shared_ptr<Entity> e);
  SimpleComponent *create_component(std::shared_ptr<Entity> e, int value);
  virtual bool remove(int64_t uuid) override;

  void clear();

  inline const auto &get_components() const { return _components; }
  // bool remove_component(SimpleComponent *c);

  void print_all_components();

  // void init() const;
  void update(const float dt);
  // void destroy() const;

private:
  std::map<int64_t, std::unique_ptr<SimpleComponent>> _components{};
};
