#pragma once

#include "includes/SimpleSystem.hpp"
#include "includes/RenderSystem.hpp"
#include "includes/RenderComponent.hpp"
#include "includes/UUIDManager.hpp"

struct SceneManager {
  SceneManager();

  void load();
  void run();
  void unload();

private:
  //   Scene current_scene; // don't default this :)
  UUIDManager _uuid_manager{};
  SimpleSystem simple_sys{};
  WindowManager _wm{};
  core::RenderSystem render_sys{&_wm};
};
