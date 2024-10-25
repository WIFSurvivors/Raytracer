#pragma once

#include "component.hpp"

template<class T>
struct simple_component : component {
  simple_component();
  simple_component(T value, bool show_log = true);
  virtual void init() override;
  virtual void update(float dt) override;
  virtual void destroy() override;
  virtual ~simple_component() override;

  void set_value(T value);
  T get_value();

  void set_update_log(bool show_log);
  bool get_update_log();

private:
  T _value{};
  bool _show_log{true};

  void print_class_content();
};
