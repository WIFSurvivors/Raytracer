#include "includes/utility/Log.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Timer.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <boost/json.hpp>
#include <cassert>
#include <format>

Scene::Scene(Engine *e)
    : _render_system{e->get_window_manager(), &_camera_system},
      _root{create_root("root")} {
  //   _render_system.init();

  generate_sample_content();
}

Scene::Scene(Engine *e, uuid id)
    : _render_system{e->get_window_manager(), &_camera_system},
      _root{create_root("root", id)} {
  // does not generate sample content
  // this should be called when loading from json
  //   _render_system.init();
}

Scene::~Scene() { _render_system.destroy(); }

std::shared_ptr<Entity> Scene::create_root(const std::string &name) {
  auto uuid = _uuid_manager.create_uuid_ownerless();
  return create_root(name, uuid);
}

std::shared_ptr<Entity> Scene::create_root(const std::string &name, uuid id) {
  LOG(std::format("scene: create_root(name, uuid): \"{}\", {}", name,
                           boost::uuids::to_string(id)));
  return _entity_storage.create_root_entity(name, id);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name) {
  auto uuid = _uuid_manager.create_uuid_ownerless();
  return create_entity(name, uuid, _root);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name,
                                             uuid uuid) {
  return create_entity(name, uuid, _root);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name,
                                             std::shared_ptr<Entity> parent) {
  return create_entity(name, _uuid_manager.create_uuid_ownerless(), parent);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name, uuid id,
                                             std::shared_ptr<Entity> parent) {
  LOG(std::format(
      "scene: create_entity(name, uuid, parent): \"{}\", {}, \"{}\"", name,
      boost::uuids::to_string(id), parent->get_name()));
  return _entity_storage.create_entity(name, id, parent);
}

void Scene::print() {
  _root->print();
}

void Scene::generate_sample_content() {
  _uuid_manager.print();
  _entity_storage.print();
  _render_system.print();
  _camera_system.print();
  _root->print();

  LOG_NEW_LINE();
  LOG(std::string(100, '*'));
  LOG_NEW_LINE();

  // ============== ENTITY + SIMPLE COMPONENT ==============

  auto e1 = create_entity("camera");
  e1->set_local_position(glm::vec3{+0, +10, +10});
  e1->set_local_rotation(glm::vec3{+0, +15, -5});
  auto e2 = create_entity("circle1");
  e2->set_local_rotation(glm::vec3{45, 0, 0});
  auto e3 = create_entity("circle2", e1);
  e3->set_local_position(glm::vec3{-2, 6, 7});

  auto new_uuid = _uuid_manager.create_uuid(&_camera_system);
  auto c1 = _camera_system.create_component(new_uuid, e1.get(), 60.f);

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
  _render_system.create_component(_uuid_manager.create_uuid(&_render_system),
                                  root_ptr.get(), v2, u2);
  _render_system.create_component(_uuid_manager.create_uuid(&_render_system),
                                  root_ptr.get(), v3, u3);

  LOG_NEW_LINE();
  LOG(std::string(100, '*'));
  LOG_NEW_LINE();

  auto sys = _uuid_manager.get_system(c1->get_uuid());
  auto csys = static_cast<CameraSystem *>(sys);
  auto occ = csys->get_component(
      c1->get_uuid()); // you ideally already know the uuid :3
  if (occ.has_value()) {
    auto cc = occ.value();
    cc->get_fov(); // ... do something with component ...
    LOG("YAY");
  }

  LOG(boost::json::serialize(c1->to_json()));

  LOG_NEW_LINE();
  LOG(std::string(100, '*'));
  LOG_NEW_LINE();

  _uuid_manager.print();
  _entity_storage.print();
  _render_system.print();
  _camera_system.print();
  _root->print();
}

// currently only tell the render system to update itself
void Scene::update(const float dt) {
//   do things??
//   _camera_system.sample_update_move_main_camera(timer.get_delta_time());
}

void Scene::render(const float total_time) {
  _render_system.update(total_time);
}


