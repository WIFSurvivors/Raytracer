#pragma once

#include "includes/Component.hpp"
#include "includes/Entity.hpp"
#include <memory>

struct SimpleComponent : Component {
  explicit SimpleComponent(std::weak_ptr<Entity> e);
  SimpleComponent(std::weak_ptr<Entity> e, int value);
  void init() override;
  void update(float dt) override;
  void destroy() override;
  // virtual ~SimpleComponent() override;

  void set_value(int value);
  int get_value();

  void print() override;

private:
  int _value{};

  void print_class_content();
};
