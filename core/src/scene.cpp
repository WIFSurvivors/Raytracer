#include "scene.hpp"
#include "entity.hpp"
#include <iostream>

scene::scene() : _root{std::make_unique<entity>()} {}

scene::~scene() { std::cout << "~scene()\n"; }

std::weak_ptr<entity> scene::get_root() { return _root; }

std::shared_ptr<entity> scene::create_entity(std::string name) {
  std::cout << "~s1()\n";
  return create_entity(name, _root, entity_counter++);
}
std::shared_ptr<entity> scene::create_entity(std::string name,
                                             std::shared_ptr<entity> parent) {
  std::cout << "~s2()\n";
  return create_entity(name, parent, entity_counter++);
}

std::shared_ptr<entity> scene::create_entity(std::string name,
                                             std::shared_ptr<entity> parent,
                                             long uuid) {
  std::cout << "~s3()\n";
  auto e = entity::create(name, uuid, parent);
  // auto ent = std::make_shared<entity>(name, parent, uuid);
  return e;
}

void scene::print() { _root->print(); }
