#pragma once

#include "includes/system/SimpleSystem.hpp"
#include "includes/system/RenderSystem.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/WindowManager.hpp"

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
  core::RenderSystem render_sys{};
};
