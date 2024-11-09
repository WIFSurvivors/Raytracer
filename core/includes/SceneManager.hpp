#pragma once

#include "includes/SimpleSystem.hpp"

struct SceneManager {
  SceneManager();

  void load_example_scene();

private:
  SimpleSystem simple_sys{};
};
