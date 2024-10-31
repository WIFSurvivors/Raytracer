#include "simple_component.hpp"

#include <iostream>
#include <string>

simple_component::simple_component() : component{} {
  std::cout << "-- dummy component constructed without parameter \n";
}

simple_component::simple_component(int value, bool show_log)
    : component{}, _value{value}, _show_log{show_log} {
  std::cout << "-- dummy component constructed with parameter ";
  print_class_content();
  std::cout << "\n";
}

void simple_component::init() { std::cout << "--- dummy_component: init\n"; }

void simple_component::update(float dt) {
  std::cout << "dummy_component: update\n";
}

void simple_component::destroy() {
  std::cout << "--- dummy_component: destroy\n";
}

simple_component::~simple_component() {
  std::cout << "-- simple_component: destructor\n";
}

void simple_component::set_update_log(bool show_log) { _show_log = show_log; }

bool simple_component::get_update_log() { return _show_log; }

void simple_component::set_value(int value) { _value = value; }

int simple_component::get_value() { return _value; }

void simple_component::print_class_content() {
  std::cout << "simple_component: " << std::to_string(_value);
}
