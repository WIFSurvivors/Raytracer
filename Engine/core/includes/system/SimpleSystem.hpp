#pragma once
#include "includes/Entity.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/system/System.hpp"
#include <boost/uuid/uuid.hpp>
#include <map>
#include <memory>

typedef boost::uuids::uuid uuid;

struct SimpleSystem : public ISystem<SimpleComponent> {
  SimpleSystem();

  SimpleComponent *create_component(uuid id, Entity *e, int value);

  void update(const float dt);

private:
  using ISystem::create_component;
  void print_component(const SimpleComponent& c) override;
};

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
