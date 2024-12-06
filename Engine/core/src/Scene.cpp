#include "includes/Scene.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <cassert>
#include <format>

Scene::Scene(Engine *e)
    : _root{create_root("root")}, _render_system{e->get_window_manager()} {
  _render_system.init();

  generate_sample_content();
}

Scene::Scene(Engine *e, uuid id)
    : _root{create_root("root", id)}, _render_system{e->get_window_manager()} {
  // does not generate sample content
  // this should be called when loading from json
  _render_system.init();
}

Scene::~Scene() { _render_system.destroy(); }

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

void Scene::generate_sample_content() {
  _uuid_manager.print();
  _entity_system.print();
  _render_system.print();
//   _simple_system.print_all_components();

  SimpleLogger::print("\n");
  SimpleLogger::print(std::string(100, '*'));
  SimpleLogger::print("\n");

  
  // ============== ENTITY + SIMPLE COMPONENT ==============

  auto e1 = create_entity("camera");
  e1->set_local_position(glm::vec3{0, 0, +10});
  auto e2 = create_entity("circle1");
  e2->set_local_rotation(glm::vec3{45, 0, 0});
  auto e3 = create_entity("circle2", e1);
  e3->set_local_position(glm::vec3{-2, 6, 7});

  auto new_uuid = _uuid_manager.getNewUUID(&_simple_system);
  auto c0 = _camera_system.create_component(new_uuid, e1.get());
  new_uuid = _uuid_manager.getNewUUID(&_simple_system);
  auto c2 = _simple_system.create_component(new_uuid, e3.get(), -56);

  // =================== RENDER SYSTEM =====================

  std::vector<glm::vec3> v1 = {
      glm::vec3{-1.0f, -1.0f, 0.0f}, glm::vec3{1.0f, -1.0f, 0.0f},
      glm::vec3{1.0f, 1.0f, 0.0f},   glm::vec3{-1.0f, -1.0f, 0.0f},
      glm::vec3{1.0f, 1.0f, 0.0f},   glm::vec3{-1.0f, 1.0f, 0.0f}};
  std::vector<glm::vec3> v2 = {glm::vec3{-1.0f, -1.0f, 0.0f},
                               glm::vec3{1.0f, -1.0f, 0.0f},
                               glm::vec3{1.0f, 1.0f, 0.0f}};
  std::vector<glm::vec3> v3 = {glm::vec3{-1.0f, -1.0f, 0.0f},
                               glm::vec3{1.0f, 1.0f, 0.0f},
                               glm::vec3{-1.0f, 1.0f, 0.0f}};
  std::vector<glm::vec2> u1 = {glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 0.0f},
                               glm::vec2{1.0f, 1.0f}, glm::vec2{0.0f, 0.0f},
                               glm::vec2{1.0f, 1.0f}, glm::vec2{0.0f, 1.0f}};
  std::vector<glm::vec2> u2 = {glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 0.0f},
                               glm::vec2{1.0f, 1.0f}};
  std::vector<glm::vec2> u3 = {glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f},
                               glm::vec2{0.0f, 1.0f}};

  auto root_ptr = get_root().lock();
  _render_system.create_component(_uuid_manager.getNewUUID(&_render_system), root_ptr.get(),
                                  v2, u2);
  _render_system.create_component(_uuid_manager.getNewUUID(&_render_system), root_ptr.get(),
                                  v3, u3);

  
  SimpleLogger::print("\n");
  SimpleLogger::print(std::string(100, '*'));
  SimpleLogger::print("\n");

  _uuid_manager.print();
  _entity_system.print();
  _render_system.print();
  _camera_system.print();
//   _simple_system.print_all_components();
}

// currently only tell the render system to update itself
void Scene::update(float dt) { _render_system.update(dt); }
