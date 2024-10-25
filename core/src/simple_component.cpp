#include "simple_component.hpp"

#include <iostream>

template <class T> simple_component<T>::simple_component() : component{} {
  std::cout << "-- dummy component constructed without parameter \n";
}

template <class T>
simple_component<T>::simple_component(T value, bool show_log) : component{}, _value{value}, _show_log{show_log} {
  std::cout << "-- dummy component constructed with parameter "
            << print_class_content() << "\n";
}

template <class T> void simple_component<T>::init() {
  std::cout << "dummy_component: init\n";
}

template <class T> void simple_component<T>::update(float dt) {
  std::cout << "dummy_component: update\n";
}

template <class T> void simple_component<T>::destroy() {
  std::cout << "dummy_component: init\n";
}

template <class T> simple_component<T>::~simple_component() {
  std::cout << "dummy_component: init\n";
}

template <class T> void simple_component<T>::set_update_log(bool show_log) { _show_log = show_log; }

template <class T> bool simple_component<T>::get_update_log() { return _show_log; }

template <class T> void simple_component<T>::set_value(T value) {_value = value;}

template <class T> T simple_component<T>::get_value() { return _value; }

template <class T> void simple_component<T>::print_class_content() {
  std::cout << typeid(T).name() << ": " << _value;
}
