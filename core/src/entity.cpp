#include "entity.hpp"

#include <vector>

entity::entity() {}

std::vector<component> &entity::get_components() { return _components; }
