#include "SceneManager.hpp"
#include "WindowEngine.h"
#include <iostream>

int main() {
  SceneManager sm{};
  sm.load_example_scene();

  WindowEngine engine{};
  engine.initializeEngine();
}
