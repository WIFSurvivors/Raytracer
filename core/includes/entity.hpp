#pragma once
#include "component.hpp"
#include <glm/glm.hpp>
#include <vector>

struct entity {
  entity();

  std::vector<component> &get_components(component c);

private:
  std::vector<component> _components{};
};
