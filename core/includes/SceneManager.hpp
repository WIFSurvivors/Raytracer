#pragma once

#include "includes/SimpleSystem.hpp"
#include "includes/RenderSystem.hpp"
#include "includes/RenderComponent.hpp"

struct SceneManager {
  SceneManager();

  void load();
  void run();
  void unload();

private:
  Scene current_scene; // don't default this :)
  SimpleSystem simple_sys{};
  WindowManager _wm{};
  core::RenderSystem render_sys{&_wm};
};
