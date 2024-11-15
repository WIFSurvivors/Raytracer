#include "includes/SceneManager.hpp"
#include "glm/vec3.hpp"
#include "includes/Scene.hpp"
#include <iostream>

SceneManager::SceneManager() { render_sys.init(); }

void SceneManager::load() {
  // optionally accessing the Entity root object
  // auto root_entity = s.get_root().lock();

  // build up a tree of entities
  // will also possible with tcp commands
  auto e1 = current_scene.create_entity("camera");
  auto e2 = current_scene.create_entity("cat", 13);
  auto e3 = current_scene.create_entity("cube", e2);

  // add components to entities
  // will also possible with tcp commands
  auto c1 = simple_sys.create_component(e1, 5);
  auto c2 = simple_sys.create_component(e1, 8);
  auto c3 = simple_sys.create_component(e2, -5);

  current_scene.print();
  simple_sys.print_all_components();
}

void SceneManager::run() {
  // all components should call init
  // debatable if this is neccessary to do for *all* components
  for (auto const &[uuid, component] : simple_sys.get_components()) {
    component->init();
  }

  if (!current_scene.get_root().expired()) {
    auto root = current_scene.get_root().lock();
    auto child_entities = root->get_child_entities();

    std::cout << "elements: " << child_entities.size() << "\n";
    child_entities[1]->get_child_entities()[0]->position = {20.f, 0.5f, -4.f};

    child_entities[0]->rotation = {0.f, 90.f, 0.f};
    std::cout << "\n\n4\n";

    if (auto c = root->get_component(1000)) { // == FALSE
      if (c.has_value()) {
        auto sc = static_cast<SimpleComponent *>(c.value());
        sc->set_value(80);
      }
    }
    if (auto c = child_entities[0]->get_component(1000)) { // == TRUE
      if (c.has_value()) {
        auto sc = static_cast<SimpleComponent *>(c.value());
        sc->set_value(80);
      }
    }
  }

  auto e4 = current_scene.create_entity("cone light");
  auto c4 = simple_sys.create_component(e4, 1);
  c4->init();

  double dt_n0 = 0., dt_n1 = 0., dt = 0.;
  do {
    dt_n0 = dt_n1;
    dt_n1 = _wm.get_time();
    dt = dt_n1 - dt_n0;

    // std::cout << "dt: " << dt << "\n";

    simple_sys.update(dt);
    // render_sys.update(dt);
  } while (true);

  std::cout << "\n\n3)\n";
  current_scene.print();
  std::cout << "\n\n\n";
  simple_sys.print_all_components();
  std::cout << "\n\n\n";
}

void SceneManager::unload() {
  // remove some possible external references
  // (here it does nothing)
  for (auto &&c : simple_sys.get_components()) {
    c.second->destroy();
  }
}
