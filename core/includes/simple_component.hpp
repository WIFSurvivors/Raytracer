#pragma once

#include "component.hpp"
#include "entity.hpp"

struct simple_component : component {
  simple_component(std::weak_ptr<entity> e);
  simple_component(std::weak_ptr<entity> e, int value);
  virtual void init() override;
  virtual void update(float dt) override;
  virtual void destroy() override;
  virtual ~simple_component() override;

  void set_value(int value);
  int get_value();

  virtual void print() override;

private:
  int _value{};

  void print_class_content();
};
