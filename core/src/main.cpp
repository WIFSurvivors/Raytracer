#include "WindowEngine.h"
#include "component.hpp"
#include "entity.hpp"
#include "simple_component.hpp"
#include "simple_system.hpp"
#include <iostream>

void example_ecs();

int main() {
  example_ecs();

  WindowEngine engine;
  engine.initializeEngine();
}

void example_ecs() {
  // simple_system is responsible for storing all simple_components<int>
  // because this is a simple system, it only prints out the values on function
  // call print(), for example: a render_system might send data to the GPU (very
  // simplified)
  simple_system sys{};
  // entity_system entity_sys{};

  {
    // create entities -> these are created by UI or JSON
    entity e1{};
    entity e2{};
    entity e3{};

    // create components -> these are created by UI or JSON
    auto c1 = sys.create_component(e1, 5);
    auto c2 = sys.create_component(e1, 8);
    auto c3 = sys.create_component(e3, -5);

    // === PREPARE PREVIEW

    // entity contain components now
    // therefore we can connect components (if required)
    sys.init();
    // system doesn't have to call init() for each component
    // it depends if components require external data etc.

    // === PREVIEW IS READY

    // now the scene is set up and is updateable
    const float dt{1.f / 30.f};

    // sys stores all components and can update them all
    sys.update(dt);
    sys.print_all_components(true);
    sys.update(dt);
    sys.print_all_components(true);
    sys.update(dt);
    sys.print_all_components(true);
    sys.update(dt);
    sys.print_all_components(true);
    sys.update(dt);
    sys.print_all_components(true);

    // PREVIEW IS BEING DESTORYED -> clean-up external references first

    sys.destroy();
  }
}
