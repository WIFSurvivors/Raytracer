#include "entity.hpp"

#include <vector>

std::vector<component *> &entity::get_components() { return _components; }

void entity::add_component(component *c) { _components.push_back(c); }
