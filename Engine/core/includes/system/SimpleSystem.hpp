#pragma once
#include "includes/Entity.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/system/System.hpp"
#include <boost/uuid/uuid.hpp>
#include <map>
#include <memory>
#include <string>

// typedef boost::uuids::uuid uuid;

struct SimpleSystem : public System<SimpleComponent> {
  SimpleSystem();

  SimpleComponent *create_component(uuid id, Entity *e, int value);

  void update(const float dt);

  void print() override;

  inline std::string get_system_name() const override {
    return "Simple System";
  }

private:
  using System::create_component_base;
  using typename System::uuid;
};

// template<class T> requires
// struct system{
//     virtual bool add_component(IComponent c) = 0;
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

//     bool add_component(derived<IComponent> c);
//     bool remove_component(derived<IComponent> c);

// private:
//     std::vector<derived<IComponent>> _components;
// };
