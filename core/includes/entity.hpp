#pragma once
#include "component.hpp"
#include <vector>

struct entity {
  std::vector<component *> &get_components();
  void add_component(component *c);
  // bool add_component(int128_t uuid);

private:
  // add name
  // add entity uuid

  // replace with component uuid vector / map
  std::vector<component *> _components{};
};
