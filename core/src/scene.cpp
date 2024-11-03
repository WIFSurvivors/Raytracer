#include "scene.hpp"

#include "entity.hpp"
scene::scene() : _root{std::make_unique<entity>()} {}

std::shared_ptr<entity> scene::get_root() { return _root; }

std::shared_ptr<entity> scene::create_entity(std::string name,
                                             std::shared_ptr<entity> parent) {
  return create_entity(name, parent, entity_counter++);
}

std::shared_ptr<entity> scene::create_entity(std::string name,
                                             std::shared_ptr<entity> parent,
                                             long uuid) {
  auto ent = std::make_shared<entity>(name, parent, uuid);

  return ent;
}

void scene::print() { _root->print(); }
