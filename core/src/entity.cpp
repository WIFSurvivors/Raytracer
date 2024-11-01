#include "entity.hpp"

#include <vector>

std::vector<component *> &entity::get_components() { return _components; }

void entity::add_component(component *c) { _components.push_back(c); }

void entity::remove_component(component *c) {
  auto it = std::find(_components.begin(), _components.end(), c);
  _components.erase(it);

  auto it_ = _components.begin();
};

void entity::remove_component(long uuid) {
  auto it =
      std::find_if(_components.begin(), _components.end(),
                   [uuid](component *c) { return c->get_uuid() == uuid; });
}
