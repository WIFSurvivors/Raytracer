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
        return false;
      }
    }
    // remove all children, should do nothing in theory
    // e->get_child_entities().clear();
  }

  // 2) Remove all components that are currently active on the removed entity
  // from their systems.
  if (!e->remove_components(_uuid_manager.get())) {
    return false;
  }

  // 3) Remove entity from it's parent
  if (auto parent = e->get_parent_entity().lock();
      !parent->remove_child_entity(e)) {
    LOG_ERROR(std::format("Removal of {} failed", e->get_name()));
  }
  // 4) Finally, remove the selected entity from storage container
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

void Scene::print_system_data() {
  _uuid_manager->print();
  _asset_manager.print();
  _entity_storage.print();
  _render_system.print();
  _camera_system.print();
  _light_system.print();
}

void Scene::generate_test_scene() {
  LOG(std::string(30, '*'));
  LOG(std::string(30, '*'));
  LOG(std::string(30, '*'));
  {
    LOG_TEST("===== START TEST SEQUENCE ====");
    {
      LOG_TEST("==[START] INIT");
      assert(_entity_storage.get_storage().size() == 1 &&
             "Entity Storage doesn't have 1 entity");
      assert(_root->get_child_entities().size() == 0 && "Root has >0 entities");
      auto e = create_entity("camera");
      assert(_entity_storage.get_storage().size() == 2 &&
             "Entity Storage doesn't have 2 entity");
      assert(_root->get_child_entities().size() == 1 && "Root has >1 entities");
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
      auto test_root_children_count = _root->get_child_entities().size();
      LOG(std::format("Expected Child count: {}", test_root_children_count));
      auto e = create_entity("light test #2").get();
      auto l = _light_system.create_component(e);
      assert(_light_system.get_components().size() == 1 &&
             "Light System doesn't have 1 component");
      assert(e->get_components().size() == 1 &&
             "Entity doesn't have 1 component");
      LOG(std::format("{}", e->get_child_entities().size()));
      this->remove(e->get_uuid());
      assert(_light_system.get_components().size() == 0 &&
             "Light System doesn't have 0 components");
      LOG(std::format("Expected Child count: {}",
                      _entity_storage.get_storage().size()));
      assert(_entity_storage.get_storage().size() == test_entity_count &&
             "Entity Storage doesn't represent the previous state");
      assert(_root->get_child_entities().size() == test_root_children_count &&
             "Root doesn't represent the previous state");
      LOG_TEST(
          "==[SUCCESS] DELETING ENTITY WITH LIGHT COMPONENT (UUID VERSION)");
    }

    {
      LOG_TEST("==[START] DELETING ENTITY WITH LIGHT COMPONENT (PTR VERSION)");
      auto test_entity_count = _entity_storage.get_storage().size();
      auto test_root_children_count = _root->get_child_entities().size();
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
      assert(_root->get_child_entities().size() == test_root_children_count &&
             "Root doesn't represent the previous state");
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
      auto test_root_children_count = _root->get_child_entities().size();
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
      assert(_root->get_child_entities().size() == test_root_children_count &&
             "Root doesn't represent the previous state");
      LOG_TEST(
          "==[SUCCESS] DELETING ENTITY WITH RENDER COMPONENT (UUID VERSION)");
    }

    {
      LOG_TEST("==[START] DELETING ENTITY WITH RENDER COMPONENT (PTR VERSION)");
      auto test_entity_count = _entity_storage.get_storage().size();
      auto test_root_children_count = _root->get_child_entities().size();
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
      assert(_root->get_child_entities().size() == test_root_children_count &&
             "Root doesn't represent the previous state");
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

// currently only tell the render system to update itself
// other system have no use for per-frame updates
void Scene::update(const FrameSnapshot &snapshot) {
  _render_system.update(snapshot);
}
} // namespace RT
