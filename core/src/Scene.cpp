#include "includes/Scene.hpp"
#include "includes/Engine.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <format>

Scene::Scene(Engine &e) : _root{create_root("root")} {}

Scene::Scene(Engine &e, uuid id) : _root{create_root("root", id)} {}

std::shared_ptr<Entity> Scene::create_root(const std::string &name) {
  auto uuid = _uuid_manager.getNewUUID(&_entity_system);
  return create_root(name, uuid);
}

std::shared_ptr<Entity> Scene::create_root(const std::string &name, uuid id) {
  SimpleLogger::print(
      std::format("-- scene: create_root(name, uuid): \"{}\", {}", name,
                  boost::uuids::to_string(id)));
  return _entity_system.create_root(name, id);
}

std::weak_ptr<Entity> Scene::get_root() { return _root; }

std::shared_ptr<Entity> Scene::create_entity(const std::string &name) {
  auto uuid = _uuid_manager.getNewUUID(&_entity_system);
  return create_entity(name, uuid, _root);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name,
                                             uuid uuid) {
  return create_entity(name, uuid, _root);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name,
                                             std::shared_ptr<Entity> parent) {
  return create_entity(name, _uuid_manager.getNewUUID(&_entity_system), parent);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name, uuid id,
                                             std::shared_ptr<Entity> parent) {
  SimpleLogger::print(std::format(
      "-- scene: create_entity(name, uuid, parent): \"{}\", {}, \"{}\"", name,
      boost::uuids::to_string(id), parent->get_name()));
  return _entity_system.create_entity(name, id, parent);
}

void Scene::print() { _root->print(); }

void Scene::generate_sample_content() { _uuid_manager.print(); }

UUIDManager *Scene::get_uuid_manager() { return &_uuid_manager; }
