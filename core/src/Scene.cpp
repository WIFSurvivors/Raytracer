#include "includes/Scene.hpp"
#include "includes/Engine.hpp"
#include "boost/uuid/uuid_io.hpp"
#include <format>

Scene::Scene(Engine &e) : _root{create_entity("root")} {
  SimpleLogger::print("scene 1");
}

std::weak_ptr<Entity> Scene::get_root() { return _root; }

std::shared_ptr<Entity> Scene::create_entity(std::string name) {
  // _entity_system.print();
  // SimpleLogger::print(
  //     std::format("EntitySystem -> {}", typeid(_entity_system)));
  SimpleLogger::print(std::format("scene: create_entity(name): \"{}\"", name));
  auto uuid = _uuid_manager.getNewUUID(&_entity_system);
  return create_entity(name, uuid, _root);
}

std::shared_ptr<Entity> Scene::create_entity(std::string name, uuid uuid) {
  SimpleLogger::print(
      std::format("scene: create_entity(name, uuid): \"{}\"", name));
  return create_entity(name, uuid, _root);
}

std::shared_ptr<Entity> Scene::create_entity(std::string name,
                                             std::shared_ptr<Entity> parent) {
  SimpleLogger::print(
      std::format("scene: create_entity(name, parent): \"{}\"", name));
  return create_entity(name, _uuid_manager.getNewUUID(&_entity_system), parent);
}

std::shared_ptr<Entity> Scene::create_entity(std::string name, uuid uuid,
                                             std::shared_ptr<Entity> parent) {
  SimpleLogger::print(
      std::format("scene: create_entity(name, uuid, parent): \"{}\"", name));
  return nullptr;
  //   return _entity_system.create_entity(name, uuid, parent);
}

void Scene::print() { _root->print(); }

void Scene::generate_sample_content() {}

UUIDManager *Scene::get_uuid_manager() { return &_uuid_manager; }
