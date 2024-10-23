#include "entity.hpp"

entity::entity() {}

std::vector<component> &entity::get_components(component c)
{
    return _components;
}

