#include "includes/utility/Log.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/FrameSnapshot.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <boost/json.hpp>
#include <cassert>
#include <format>
#include <ostream>
#include <random>

namespace RT {
Scene::Scene(Engine *e)
    : _render_system{_uuid_manager, e->get_window_manager(), &_camera_system,
                     &_light_system, &_default_assets},
      _root{create_root("root")} {
  LOG(std::format("NEW SCENE -> \"{}\" w/ ROOT UUID {}", _title,
                  boost::uuids::to_string(_root->get_uuid())));
}

Scene::Scene(Engine *e, std::string title)
    : _render_system{_uuid_manager, e->get_window_manager(), &_camera_system,
                     &_light_system, &_default_assets},
      _root{create_root("root")}, _title{title} {
  LOG(std::format("**CREATE** SCENE -> \"{}\" w/ ROOT UUID {}", _title,
                  boost::uuids::to_string(_root->get_uuid())));
}

Scene::~Scene() {
  LOG(std::format("**DESTROY** SCENE -> \"{}\" w/ ROOT UUID {}", _title,
                  boost::uuids::to_string(_root->get_uuid())));
}

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

bool Scene::remove(Entity *e) {
  LOG(std::format("Trying to remove {} w/ {} children", e->get_name(),
                  e->get_child_entities().size()));
  // 0) Removing root is not allowed
  if (e == get_root().lock().get()) {
    LOG_ERROR(std::format("Removal of {} not allowed!!", e->get_name()));
    return false;
  }

  // 1) Recursively call remove on children
  if (e->get_child_entities().size() > 0) {
    for (auto &&ce : e->get_child_entities()) {
      if (!remove(ce.get())) {
        LOG_ERROR(std::format("Removal of {} failed", e->get_name()));
      }
    }
    e->get_child_entities().clear(); // remove all children
  }

  // 2) Remove all components that are currently active on the removed entity
  // from their systems.
  e->remove_components(_uuid_manager.get());

  // 3) Finally, remove the selected entity from storage container
  _entity_storage.remove(e);
  return true;
}

bool Scene::remove(uuid id) {
  auto e = _entity_storage.get_entity(id);
  if (!e.has_value()) {
    return false;
  }
  return remove(e.value());
}

void Scene::print() { _root->print(); }
void Scene::print_system_data() {
  _uuid_manager->print();
  _asset_manager.print();
  _entity_storage.print();
  _render_system.print();
  _camera_system.print();
  _light_system.print();
}

void Scene::generate_test() {
  LOG(std::string(30, '*'));
  LOG(std::string(30, '*'));
  LOG(std::string(30, '*'));
  {
    LOG_TEST("===== START TEST SEQUENCE ====");
    {
      LOG_TEST("==[START] INIT");
      assert(_uuid_manager->get_storage().size() == (3 + 1) &&
             "UUID Manager doesn't have 4 uuids");
      assert(_entity_storage.get_storage().size() == 1 &&
             "Entity Storage doesn't have 1 entity");
      auto e = create_entity("camera");
      assert(_entity_storage.get_storage().size() == 2 &&
             "Entity Storage doesn't have 2 entity");
      auto c = _camera_system.create_component(e.get());
      assert(_camera_system.get_components().size() == 1 &&
             "Camera Syteme doesn't have 1 component");
      e->set_local_position(0.f, +8.f, 15.f);
      LOG_TEST("==[SUCCESS] INIT SUCCESSFULL");
    }

    {
      LOG_TEST("==[START] ROOT DELETION PREVENTED");
      auto root = this->get_root().lock().get();
      this->remove(root->get_uuid());
      assert(root != nullptr && "Root entity has been deleted!!");
      LOG_TEST("==[SUCCESS] ROOT DELETION PREVENTED");
    }

    {
      LOG_TEST("==[START] MAIN CAMERA DELETION PREVENTED");
      auto c = _camera_system.get_main_camera();
      assert(c != nullptr && "Camera Syteme doesn't have a main camera");
      _camera_system.remove(
          c->get_uuid()); // this will not be successfull -> THIS IS INTENDED
      assert(_camera_system.get_components().size() == 1 &&
             "Camera Syteme doesn't have 1 component");
      LOG_TEST("==[SUCCESS] MAIN CAMERA DELETION PREVENTED");
    }

    {
      LOG_TEST("==[START] DELETING LIGHT COMPONENT");
      auto e = create_entity("light test #1").get();
      auto l = _light_system.create_component(e);
      assert(_light_system.get_components().size() == 1 &&
             "Light System doesn't have 1 component");
      assert(e->get_components().size() == 1 &&
             "Entity doesn't have 1 component");
      auto s = _uuid_manager->get_storage(l->get_uuid());
      s->remove(l->get_uuid());
      assert(_light_system.get_components().size() == 0 &&
             "Light System doesn't have 0 components");
      assert(e->get_components().size() == 0 &&
             "Entity doesn't have 0 components");
      LOG_TEST("==[SUCCESS] DELETING LIGHT COMPONENT");
    }

    {
      LOG_TEST("==[START] DELETING ENTITY WITH LIGHT COMPONENT (UUID VERSION)");
      auto test_entity_count = _entity_storage.get_storage().size();
      auto e = create_entity("light test #2").get();
      auto l = _light_system.create_component(e);
      assert(_light_system.get_components().size() == 1 &&
             "Light System doesn't have 1 component");
      assert(e->get_components().size() == 1 &&
             "Entity doesn't have 1 component");
      this->remove(e->get_uuid());
      assert(_light_system.get_components().size() == 0 &&
             "Light System doesn't have 0 components");
      assert(_entity_storage.get_storage().size() == test_entity_count &&
             "Entity Storage doesn't represent the previous state");
      LOG_TEST(
          "==[SUCCESS] DELETING ENTITY WITH LIGHT COMPONENT (UUID VERSION)");
    }

    {
      LOG_TEST("==[START] DELETING ENTITY WITH LIGHT COMPONENT (PTR VERSION)");
      auto test_entity_count = _entity_storage.get_storage().size();
      auto e = create_entity("light test #3").get();
      auto l = _light_system.create_component(e);
      assert(_light_system.get_components().size() == 1 &&
             "Light System doesn't have 1 component");
      assert(e->get_components().size() == 1 &&
             "Entity doesn't have 1 component");
      this->remove(e);
      assert(_light_system.get_components().size() == 0 &&
             "Light System doesn't have 0 components");
      assert(_entity_storage.get_storage().size() == test_entity_count &&
             "Entity Storage doesn't represent the previous state");
      LOG_TEST(
          "==[SUCCESS] DELETING ENTITY WITH LIGHT COMPONENT (PTR VERSION)");
    }

    {
      LOG_TEST("==[START] DELETING RENDER COMPONENT");
      auto e = create_entity("render test #1").get();
      auto a = create_asset("./assets/cornell-box.obj");
      auto r = _render_system.create_component(e, a);
      assert(_render_system.get_components().size() == 1 &&
             "Render System doesn't have 1 component");
      assert(e->get_components().size() == 1 &&
             "Entity doesn't have 1 component");
      auto s = _uuid_manager->get_storage(r->get_uuid());
      s->remove(r->get_uuid());
      assert(_render_system.get_components().size() == 0 &&
             "Render System doesn't have 0 components");
      assert(e->get_components().size() == 0 &&
             "Entity doesn't have 0 components");
      LOG_TEST("==[SUCCESS] DELETING RENDER COMPONENT");
    }

    {
      LOG_TEST(
          "==[START] DELETING ENTITY WITH RENDER COMPONENT (UUID VERSION)");
      auto test_entity_count = _entity_storage.get_storage().size();
      auto e = create_entity("render test #2").get();
      auto a = create_asset("./assets/cornell-box.obj");
      auto r = _render_system.create_component(e, a);
      assert(_render_system.get_components().size() == 1 &&
             "Render System doesn't have 1 component");
      assert(e->get_components().size() == 1 &&
             "Entity doesn't have 1 component");
      this->remove(e->get_uuid());
      assert(_render_system.get_components().size() == 0 &&
             "Render System doesn't have 0 components");
      assert(_entity_storage.get_storage().size() == test_entity_count &&
             "Entity Storage doesn't represent the previous state");
      LOG_TEST(
          "==[SUCCESS] DELETING ENTITY WITH RENDER COMPONENT (UUID VERSION)");
    }

    {
      LOG_TEST("==[START] DELETING ENTITY WITH RENDER COMPONENT (PTR VERSION)");
      auto test_entity_count = _entity_storage.get_storage().size();
      auto e = create_entity("render test #3").get();
      auto a = create_asset("./assets/cornell-box.obj");
      auto r = _render_system.create_component(e, a);
      assert(_render_system.get_components().size() == 1 &&
             "Render System doesn't have 1 component");
      assert(e->get_components().size() == 1 &&
             "Entity doesn't have 1 component");
      this->remove(e);
      assert(_render_system.get_components().size() == 0 &&
             "Render System doesn't have 0 components");
      assert(_entity_storage.get_storage().size() == test_entity_count &&
             "Entity Storage doesn't represent the previous state");
      LOG_TEST(
          "==[SUCCESS] DELETING ENTITY WITH RENDER COMPONENT (PTR VERSION)");
    }

    {
      LOG_TEST("==[START] RANDOMIZED Y-POSIION VISUAL TEST");
      // Create a random device and use it to seed the random number generator
      std::random_device rd;
      std::mt19937 gen(rd()); // Mersenne Twister engine
      // Define the range [-15, 0]
      std::uniform_real_distribution<> dis(-15.0, 0.0);
      // Generate a random float
      float random_float = dis(gen);

      auto e = create_entity("randomized y-position visual test").get();
      e->set_local_position(0.f, random_float, 0.f);
      auto a = create_asset("./assets/default.obj");
      auto r = _render_system.create_component(e, a);
      assert(_render_system.get_components().size() == 1 &&
             "Render System doesn't have 1 component");
      assert(e->get_components().size() == 1 &&
             "Entity doesn't have 1 component");
      LOG_TEST("==[SUCCESS] RANDOMIZED Y-POSIION VISUAL TEST");
    }
    LOG_TEST("===== FINISHED TEST SEQUENCE ====");
  }
  print_system_data();
  LOG(std::string(30, '*'));
  LOG(std::string(30, '*'));
  LOG(std::string(30, '*'));
}

void Scene::generate_sample_content() {
  print_system_data();

  LOG_NEW_LINE();
  LOG(std::string(100, '*'));
  LOG_NEW_LINE();

  auto a = create_asset("test");

  LOG(std::format("1) Asset {} {}", a.get_path().string(),
                  boost::uuids::to_string(a.get_uuid())));
  auto b = create_asset("test");
  LOG(std::format("2) Asset {} {}", b.get_path().string(),
                  boost::uuids::to_string(b.get_uuid())));
  AssetManager::Asset c{&_asset_manager, "test"};
  LOG(std::format("3) Asset {} {}", c.get_path().string(),
                  boost::uuids::to_string(c.get_uuid())));
  AssetManager::Asset d{&_asset_manager, "test2"};
  LOG(std::format("4) Asset {} {}", d.get_path().string(),
                  boost::uuids::to_string(d.get_uuid())));

  LOG_NEW_LINE();
  LOG(std::string(100, '*'));
  LOG_NEW_LINE();

  // =================== ENTITIES =====================
  auto e1 = create_entity("camera");
  e1->set_local_position(glm::vec3{0.f, +8.f, 15.f});
  auto e2 = create_entity("light sources");
  auto e3 = create_entity("light red", e2);
  e3->set_local_position(glm::vec3{0.0, 3.0, -3.0});
  auto e4 = create_entity("light green", e2);
  e4->set_local_position(glm::vec3{0, -5, 5});
  auto e5 = create_entity("light blue", e2);
  e5->set_local_position(glm::vec3{0, 0, 5});

  // =================== CAMERA =====================
  auto c1 = _camera_system.create_component(e1.get(), 60.f);
  c1->get_entity()->set_local_rotation(0.0f, 7.0f, 0.0f);
  // =================== LIGHT =====================
  auto c2 = _light_system.create_component(e3.get());
  /*c2->set_color(0.8576f, 0.1f, 0.1f);*/
  c2->set_color(1.0f, 1.0f, 1.0f);
  c2->set_intensity(10.f);

  auto c3 = _light_system.create_component(e4.get());
  /*c3->set_color(0.1f, 0.96752f, 0.1f);*/
  c3->set_color(1.0f, 1.0f, 1.0f);
  c3->set_intensity(10.f);

  auto c4 = _light_system.create_component(e5.get());
  /*c4->set_color(0.1f, 0.1f, 1.f);*/
  c4->set_color(1.0f, 1.0f, 1.0f);
  c4->set_intensity(10.f);

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
  auto asset1 = create_asset("./assets/cornell-box.obj");
  auto ComponentEntity1 = create_entity("ComponentEntity1", root_ptr);
  _render_system.create_component(ComponentEntity1.get(), asset1);
  //   _render_system.create_component(root_ptr.get(), v3, u3);

  LOG_NEW_LINE();
  LOG(std::string(100, '*'));
  LOG_NEW_LINE();

  // =================== SAMPLE MANIPULATION =====================

  auto sys = _uuid_manager->get_storage(c1->get_uuid());
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
  _render_system.update(snapshot);
}
} // namespace RT
