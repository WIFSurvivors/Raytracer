#include "scene_manager.hpp"
#include <scene.hpp>

#include <iostream>

scene_manager::scene_manager() {}

void scene_manager::load_example_scene() {
  std::cout << " === EXAMPLE SCENE | START ===\n";
  scene s{};
  auto root_entity = s.get_root();

  auto e1 = s.create_entity("a", root_entity);
  auto e2 = s.create_entity("b", root_entity);
  auto e3 = s.create_entity("c", e2);

  auto c1 = s_sys.create_component(e1, 5);
  auto c2 = s_sys.create_component(e1, 8);
  auto c3 = s_sys.create_component(e3, -5);

  s.print();
  std::cout << " === EXAMPLE SCENE | END ===\n";

  //   // now the scene is set up and is updateable
  //   const float dt{1.f / 30.f};

  //   // sys stores all components and can update them all
  //   s_sys.update(dt);
  //   s_sys.print_all_components();
  //   s_sys.update(dt);
  //   s_sys.print_all_components();
  //   s_sys.update(dt);
  //   s_sys.print_all_components();
  //   s_sys.update(dt);
  //   s_sys.print_all_components();
  //   s_sys.update(dt);
  //   s_sys.print_all_components();

  //   // PREVIEW IS BEING DESTORYED -> clean-up external references first

  //   //   sys.destroy();
}
