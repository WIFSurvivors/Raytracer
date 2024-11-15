#include "includes/Scene.hpp"
#include "includes/Entity.hpp"
#include <iostream>
#include <memory>
#include <string>

Scene::Scene() : _root{std::make_shared<Entity>()} {}

std::weak_ptr<Entity> Scene::get_root() { return _root; }

// std::shared_ptr<Entity> Scene::create_entity(std::string name) {
//   return create_entity(name, entity_counter++, _root);
// }

// std::shared_ptr<Entity> Scene::create_entity(std::string name, int64_t uuid)
// {
//   return create_entity(name, uuid, _root);
// }

// std::shared_ptr<Entity> Scene::create_entity(std::string name,
//                                              std::shared_ptr<Entity> parent)
//                                              {
//   return create_entity(name, entity_counter++, parent);
// }

// std::shared_ptr<Entity> Scene::create_entity(std::string name, int64_t uuid,
//                                              std::shared_ptr<Entity> parent)
//                                              {
//   return Entity::create(name, uuid, parent);
// }

void Scene::print() { _root->print(); }
