#include "WindowEngine.h"
#include "scene_manager.hpp"
#include <iostream>

#include <simple_system.hpp>

void example_ecs();

int main() {
  // {
  scene_manager sm{};
  sm.load_example_scene();
  // }

  WindowEngine engine;
  engine.initializeEngine();
}
