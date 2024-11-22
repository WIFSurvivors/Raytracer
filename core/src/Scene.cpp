#include "includes/Scene.hpp"
#include "includes/Engine.hpp"

Scene::Scene(Engine &e)
    : _uuid_manager{e.get_uuid_manager()},
      _root{std::make_shared<Entity>(
          "root", _uuid_manager->getNewUUID(&entity_system))} {}

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

void Scene::generate_sample_content() {}
