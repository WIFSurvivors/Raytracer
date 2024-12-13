#pragma once

#include "includes/component/Component.hpp"
#include "includes/Entity.hpp"

struct SimpleComponent : public IComponent {
  //   explicit SimpleComponent(std::weak_ptr<Entity> e);
  SimpleComponent(uuid id, Entity *e);
  SimpleComponent(uuid id, Entity *e, int value);

  void update(const float dt) override;

  inline void set_value(int value) { _value = value; };
  inline int get_value() { return _value; }

private:
  int _value{};
};
