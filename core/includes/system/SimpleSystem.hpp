#pragma once
#include "includes/utility/SimpleLogger.hpp"
#include "includes/Entity.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/system/System.hpp"
#include "boost/uuid/uuid.hpp"
#include <map>
#include <memory>

typedef boost::uuids::uuid uuid;

struct SimpleSystem : public System {
  SimpleSystem();

  SimpleComponent *create_component(uuid id, Entity *e) override;
  SimpleComponent *create_component(uuid id, Entity *e, int value);
  bool remove(Component *c) override;
  bool remove(uuid uuid) override;

  void clear();

  inline const auto &get_components() const { return _components; }
  // bool remove_component(SimpleComponent *c);

  void print_all_components();

  // void init() const;
  void update(const float dt);
  // void destroy() const;

private:
  std::map<uuid, std::unique_ptr<SimpleComponent>> _components{};
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
