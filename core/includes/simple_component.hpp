#pragma once

#include "component.hpp"

struct simple_component : component {
  simple_component();
  simple_component(int value, bool show_log = true);
  virtual void init() override;
  virtual void update(float dt) override;
  virtual void destroy() override;
  virtual ~simple_component() override;

  void set_value(int value);
  int get_value();

  void set_update_log(bool show_log);
  bool get_update_log();

private:
  int _value{};
  bool _show_log{true};

  void print_class_content();
};
