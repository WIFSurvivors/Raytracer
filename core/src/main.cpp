#include "WindowEngine.h"
#include "component.hpp"
#include "entity.hpp"
#include "simple_component.hpp"
#include "simple_system.hpp"

void example_ecs();

int main() {
  example_ecs();

  WindowEngine engine;
  engine.initializeEngine();
}

void example_ecs() {
  // simple_system<int> is responsible for storing all simple_components<int>
  // because this is a simple system, it only prints out the values on function
  // call print() for example: a render_system might send data to the GPU (very
  // simplified)
  simple_system sys{};
  // entity_system entity_sys{};

  // create entities -> these is created by UI or JSON
  entity e1{};
  entity e2{};
  entity e3{};

  // create components -> these is created by UI or JSON
  auto c1 = sys.create_component(e1, 5);
  auto c2 = sys.create_component(e1, 8);
  auto c3 = sys.create_component(e3, -5);

  e1.get_components().assign({*c1, *c2});
  e3.get_components().assign({*c3});

  sys.print_all_components();
}
