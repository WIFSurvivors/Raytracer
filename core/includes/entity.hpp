#pragma once
#include "component.hpp"
#include <vector>

struct entity {
  entity();

  std::vector<component> &get_components();

private:
  std::vector<component> _components{};
};
