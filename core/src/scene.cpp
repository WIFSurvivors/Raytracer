#include "scene.hpp"
#include "entity.hpp"
#include <iostream>
#include <memory>
#include <string>

scene::scene() : _root{std::make_shared<entity>()} {}

std::weak_ptr<entity> scene::get_root() { return _root; }

std::shared_ptr<entity> scene::create_entity(std::string name) {
  return create_entity(name, entity_counter++, _root);
}

std::shared_ptr<entity> scene::create_entity(std::string name, int64_t uuid) {
  return create_entity(name, uuid, _root);
}

std::shared_ptr<entity> scene::create_entity(std::string name,
                                             std::shared_ptr<entity> parent) {
  return create_entity(name, entity_counter++, parent);
}

std::shared_ptr<entity> scene::create_entity(std::string name, int64_t uuid,
                                             std::shared_ptr<entity> parent) {
  return entity::create(name, uuid, parent);
}

void scene::print() { _root->print(); }
