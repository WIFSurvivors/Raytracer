#include "includes/SceneManager.hpp"
#include "includes/WindowEngine.h"
#include <iostream>

int main() {
  SceneManager sm{};
  sm.load_example_scene();

  WindowEngine engine{};
  engine.initializeEngine();
}
