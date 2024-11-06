#include "scene_manager.hpp"
#include "glm/vec3.hpp"
#include <scene.hpp>

#include <iostream>

scene_manager::scene_manager() {}

void scene_manager::load_example_scene() {
  // can be read from a json file
  // otherwise an "unnamed" scene is used for communicating via tcp
  scene s{};
  {
    // optionally accessing the entity root object
    auto root_entity = s.get_root().lock();

    // build up a tree of entities
    // will also possible with tcp commands
    auto e1 = s.create_entity("camera");
    auto e2 = s.create_entity("cat", 13);
    auto e3 = s.create_entity("cube", e2);

    // add components to entities
    // will also possible with tcp commands
    auto c1 = simple_sys.create_component(e1, 5);
    auto c2 = simple_sys.create_component(e1, 8);
    auto c3 = simple_sys.create_component(e2, -5);

    std::cout << "\n\n1)\n";
    s.print();
    std::cout << "\n\n\n";
    simple_sys.print_all_components();
    std::cout << "\n\n\n";

    // ===== RUNTIME BEGINS (PREVIEW)

    // initialize some possible external references
    // (here it does nothing)
    c1->init();
    c2->init();
    c3->init();

    // start updating the scene
    // delta time is for rendering not interesting
    // but can be used for camera movement inside preview
    const float dt{1.f / 30.f};
    simple_sys.update(dt);
    // c1->update(dt);
    // c2->update(dt);
    // c3->update(dt);

    // component and entities values can be changed on runtime
    e3->position = /*glm::vec3*/ {20.f, 0.5f, -4.f};
    e2->rotation = {0.f, 90.f, 0.f};
    c2->set_value(80);

    // add a new "light" entity and component on runtime
    std::cout << "\n";
    auto e4 = s.create_entity("cone light");
    auto c4 = simple_sys.create_component(e4, 1);
    c4->init();
    std::cout << "\n";

    simple_sys.update(dt);

    std::cout << "\n\n2)\n";
    s.print();
    std::cout << "\n\n\n";
    simple_sys.print_all_components();
    std::cout << "\n\n\n";

    // remove some possible external references
    // (here it does nothing)
    c1->destroy();
    c2->destroy();
    c3->destroy();
  }
}
