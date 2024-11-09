#pragma once

#include "Component.hpp"
#include "Entity.hpp"

struct SimpleComponent : Component {
  SimpleComponent(std::weak_ptr<Entity> e);
  SimpleComponent(std::weak_ptr<Entity> e, int value);
  virtual void init() override;
  virtual void update(float dt) override;
  virtual void destroy() override;
  // virtual ~SimpleComponent() override;

  void set_value(int value);
  int get_value();

  virtual void print() override;

private:
  int _value{};

  void print_class_content();
};
