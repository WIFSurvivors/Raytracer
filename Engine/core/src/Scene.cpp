#include "includes/Scene.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <cassert>
#include <format>

Scene::Scene(Engine &e) : _root{create_root("root")} {
  generate_sample_content();
}

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

bool Scene::remove(uuid id) {
  throw NotImplementedError();
  // return false;
}

void Scene::print() { _root->print(); }

void Scene::generate_sample_content() {
  _uuid_manager.print();
  _entity_system.print();

  SimpleLogger::print(std::string(10, '-'));
  auto e1 = create_entity("camera");
  auto e2 = create_entity("cat");
  auto e3 = create_entity("cube", e1);
  SimpleLogger::print(std::string(10, '-'));

  _uuid_manager.print();
  _entity_system.print();

SimpleLogger::print(std::string(10, '-'));
  auto new_uuid = _uuid_manager.getNewUUID(&_simple_system);
  // auto c0 = _render_system.create_component(e1, r, 2);
  SimpleLogger::print(std::format("wawa {}", e1.get() != nullptr));
  auto c1 = _simple_system.create_component(new_uuid, e1.get());
  new_uuid = _uuid_manager.getNewUUID(&_simple_system);
  SimpleLogger::print(std::format("wawa {}", e3.get() != nullptr));
  auto c2 = _simple_system.create_component(new_uuid, e3.get());
  //   auto c1 = _simple_system.create_component(e1, 1);
  SimpleLogger::print(std::string(10, '-'));
}

UUIDManager *Scene::get_uuid_manager() { return &_uuid_manager; }