#include "includes/RenderSystem.hpp"
#include "includes/RenderComponent.hpp"

#include "includes/SceneManager.hpp"

int main() {
  SceneManager sm{};
  sm.load();
  sm.run();
  sm.unload();

  /*
  core::RenderSystem rs;
  WindowManager wm;
  core::RenderSystem rs{&wm};

  core::RenderComponent comp;
  core::RenderComponent comp02;
  rs._component = std::make_unique<core::RenderComponent>(comp);
  rs.init();

  rs.render(); // shouldn't exist

  rs.destroy();
  */
}
