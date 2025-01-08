#include "includes/utility/Log.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/FrameSnapshot.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <boost/json.hpp>
#include <cassert>
#include <format>

namespace RT {
Scene::Scene(Engine *e)
    : _render_system{&_uuid_manager, e->get_window_manager(), &_camera_system,
                     &_light_system, &_default_assets},
      _root{create_root("root")} {}

Scene::Scene(Engine *e, std::string title)
    : _render_system{&_uuid_manager, e->get_window_manager(), &_camera_system,
                     &_light_system, &_default_assets},
      _root{create_root("root")}, _title{title} {}

Scene::~Scene() { _render_system.destroy(); }

std::shared_ptr<Entity> Scene::create_root(const std::string &name) {
  return _entity_storage.create_root_entity(name);
}

std::shared_ptr<Entity> Scene::create_root(const std::string &name, uuid id) {
  return _entity_storage.create_root_entity(name, id);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name) {
  return _entity_storage.create_entity(name, _root);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name, uuid id) {
  return _entity_storage.create_entity(name, id, _root);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name,
                                             std::shared_ptr<Entity> parent) {
  return _entity_storage.create_entity(name, parent);
}

std::shared_ptr<Entity> Scene::create_entity(const std::string &name, uuid id,
                                             std::shared_ptr<Entity> parent) {
  return _entity_storage.create_entity(name, id, parent);
}

void Scene::print() { _root->print(); }
void Scene::print_system_data() {
  _uuid_manager.print();
  _asset_manager.print();
  _entity_storage.print();
  _render_system.print();
  _camera_system.print();
  _light_system.print();
  _root->print();
}

void Scene::generate_sample_content() {
  print_system_data();

  LOG_NEW_LINE();
  LOG(std::string(100, '*'));
  LOG_NEW_LINE();

  auto a = create_asset("test");

  LOG(std::format("1) Asset {} {}", a._path.string(),
                  boost::uuids::to_string(a._uuid)));
  auto b = create_asset("test");
  LOG(std::format("2) Asset {} {}", b._path.string(),
                  boost::uuids::to_string(b._uuid)));
  AssetManager::Asset c{&_asset_manager, "test"};
  LOG(std::format("3) Asset {} {}", c._path.string(),
                  boost::uuids::to_string(c._uuid)));
  AssetManager::Asset d{&_asset_manager, "test2"};
  LOG(std::format("4) Asset {} {}", d._path.string(),
                  boost::uuids::to_string(d._uuid)));

  LOG_NEW_LINE();
  LOG(std::string(100, '*'));
  LOG_NEW_LINE();

  // =================== ENTITIES =====================
  auto e1 = create_entity("camera");
  e1->set_local_position(glm::vec3{0.f, +8.f, 15.f});
  auto e2 = create_entity("light sources");
  auto e3 = create_entity("light red", e2);
  e3->set_local_position(glm::vec3{0, 4, 0});
  auto e4 = create_entity("light green", e2);
  e4->set_local_position(glm::vec3{0, -5, 5});
  auto e5 = create_entity("light blue", e2);
  e5->set_local_position(glm::vec3{0, 0, 5});

  // =================== CAMERA =====================
  auto c1 = _camera_system.create_component(e1.get(), 60.f);

  // =================== LIGHT =====================
  auto c2 = _light_system.create_component(e3.get());
  /*c2->set_color(0.8576f, 0.1f, 0.1f);*/
  c2->set_color(1.0f, 1.0f, 1.0f);
  c2->set_intensity(25.f);

  auto c3 = _light_system.create_component(e4.get());
  /*c3->set_color(0.1f, 0.96752f, 0.1f);*/
  c3->set_color(1.0f, 1.0f, 1.0f);
  c3->set_intensity(5.f);

  auto c4 = _light_system.create_component(e5.get());
  /*c4->set_color(0.1f, 0.1f, 1.f);*/
  c4->set_color(1.0f, 1.0f, 1.0f);
  c4->set_intensity(5.f);

  // =================== RENDER =====================
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
  auto asset1 = create_asset("./assets/default.obj");
  auto asset2 = create_asset("./assets/default.obj");
  auto ComponentEntity1 = create_entity("ComponentEntity1", root_ptr);
  auto ComponentEntity2 = create_entity("ComponentEntity2", root_ptr);
  _render_system.create_component(ComponentEntity1.get(),asset1);
  _render_system.create_component(ComponentEntity2.get(),asset2);
  //_render_system.create_component(root_ptr.get(), v3, u3);

  LOG_NEW_LINE();
  LOG(std::string(100, '*'));
  LOG_NEW_LINE();

  // =================== SAMPLE MANIPULATION =====================

  auto sys = _uuid_manager.get_storage(c1->get_uuid());
  auto csys = static_cast<CameraSystem *>(sys);
  auto occ = csys->get_component(c1->get_uuid());
  if (occ.has_value()) {
    auto cc = occ.value();
    cc->get_fov(); // ... do something with component ...
    LOG("YAY");
  }

  LOG(boost::json::serialize(c1->to_json()));

  LOG_NEW_LINE();
  LOG(std::string(100, '*'));
  LOG_NEW_LINE();

  print_system_data();
}

// currently only tell the render system to update itself
void Scene::update(const FrameSnapshot &snapshot) {
  //   do things??
  //   _camera_system.sample_update_move_main_camera(timer.get_delta_time());
  _render_system.update(snapshot);
}
} // namespace RT
