#include "includes/Scene.hpp"

Scene::Scene() : _root{std::make_shared<Entity>()} {}

std::weak_ptr<Entity> Scene::get_root() { return _root; }

std::shared_ptr<Entity> Scene::create_entity(std::string name) {
  return create_entity(name, uuid{}, _root); // FIX UUID
}

std::shared_ptr<Entity> Scene::create_entity(std::string name, uuid uuid) {
  return create_entity(name, uuid, _root);
}

std::shared_ptr<Entity> Scene::create_entity(std::string name,
                                             std::shared_ptr<Entity> parent) {
  return create_entity(name, uuid{}, parent); // FIX UUID
}

std::shared_ptr<Entity> Scene::create_entity(std::string name, uuid uuid,
                                             std::shared_ptr<Entity> parent) {
  return entity_system.create_entity(name, uuid, parent);
}

void Scene::print() { _root->print(); }
