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
  e1->set_local_position(glm::vec3{0, 1, 2});
  auto e2 = create_entity("cat");
  e2->set_local_rotation(glm::vec3{45, 0, 0});
  auto e3 = create_entity("cube", e1);
  e3->set_local_position(glm::vec3{-2, 6, 7});

  SimpleLogger::print(std::string(10, '-'));

  _uuid_manager.print();
  _entity_system.print();

  SimpleLogger::print(std::string(10, '-'));

  auto new_uuid = _uuid_manager.getNewUUID(&_simple_system);
  auto c1 = _simple_system.create_component(new_uuid, e1.get());
  new_uuid = _uuid_manager.getNewUUID(&_simple_system);
  auto c2 = _simple_system.create_component(new_uuid, e3.get(), -56);

  SimpleLogger::print(std::string(10, '-'));

  _uuid_manager.print();
  _entity_system.print();
  _simple_system.print_all_components();
}

UUIDManager *Scene::get_uuid_manager() { return &_uuid_manager; }

// WORK IN PROGRESS WIP TODO WOOHOO
void Scene::update(float dt) {
  // all components should call init
  // debatable if this is neccessary to do for *all* components
  //   for (auto const &[uuid, component] : simple_sys.get_components()) {
  //     component->init();
  //   }

  //   if (!current_scene.get_root().expired()) {
  //     auto root = current_scene.get_root().lock();
  //     auto child_entities = root->get_child_entities();

  //     std::cout << "elements: " << child_entities.size() << "\n";
  //     child_entities[1]->get_child_entities()[0]->position = {20.f, 0.5f,
  //     -4.f};

  //     child_entities[0]->rotation = {0.f, 90.f, 0.f};
  //     std::cout << "\n\n4\n";

  //     if (auto c = root->get_component(1000)) { // == FALSE
  //       if (c.has_value()) {
  //         auto sc = static_cast<SimpleComponent *>(c.value());
  //         sc->set_value(80);
  //       }
  //     }
  //     if (auto c = child_entities[0]->get_component(1000)) { // == TRUE
  //       if (c.has_value()) {
  //         auto sc = static_cast<SimpleComponent *>(c.value());
  //         sc->set_value(80);
  //       }
  //     }
  //   }

  //   auto e4 = current_scene.create_entity("cone light");
  //   auto c4 = simple_sys.create_component(e4, 1);
  //   c4->init();

  //   double dt_n0 = 0., dt_n1 = 0., dt = 0.;
  //   do {
  //     dt_n0 = dt_n1;
  //     dt_n1 = _wm.get_time();
  //     dt = dt_n1 - dt_n0;

  //     // std::cout << "dt: " << dt << "\n";

  //     simple_sys.update(dt);
  //     render_sys.update(dt);
  //   } while (true);

  //   std::cout << "\n\n3)\n";
  //   current_scene.print();
  //   std::cout << "\n\n\n";
  //   simple_sys.print_all_components();
  //   std::cout << "\n\n\n";

  // remove some possible external references
  // (here it does nothing)
  //   for (auto &&c : simple_sys.get_components()) {
  //     c.second->destroy();
  //   }
}
