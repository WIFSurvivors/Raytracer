#pragma once

#include "entity.hpp"
#include "simple_system.hpp"

struct scene_manager {
  scene_manager();

  void load_example_scene();

private:
  simple_system simple_sys{};
};
