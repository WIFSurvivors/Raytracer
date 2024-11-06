#include "scene_manager.hpp"
#include <scene.hpp>

#include <iostream>

scene_manager::scene_manager() {}

scene_manager::~scene_manager() { std::cout << "~scene_manager()\n"; }

void scene_manager::load_example_scene() {
  {
    simple_system s_sys{};
    {
      scene s{};
      {
        s_sys.clear();
        // auto root_entity = s.get_root();

        // auto e1 = s.create_entity("a");
        // auto e2 = s.create_entity("b");
        // auto e3 = s.create_entity("c", e2);

        /*auto c0 = */ s_sys.create_component(s.get_root().lock(), -1);
        // auto c1 = s_sys.create_component(e1, 5);
        // auto c2 = s_sys.create_component(e1, 8);
        // auto c3 = s_sys.create_component(e3, -5);

        s_sys.print_all_components();
        // s_sys.clear();
        s_sys.print_all_components();
        // c1->set_value(80);

        // s.print();
        std::cout << " === EXAMPLE SCENE | END ===\n";
      }
      // s_sys.clear();
      // s_sys.print_all_components();
      // s.print();
    }
    s_sys.clear();
    s_sys.print_all_components();
    std::cout << " === wawa ===\n";
    // auto t = s_sys.get_components().at(1000).get();
    // std::cout << t->get_uuid() << " | " << t->get_value() << "\n";
  }
  std::cout << " === wawa 2===\n";

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
