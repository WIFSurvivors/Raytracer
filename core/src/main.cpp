#include "includes/RenderSystem.hpp"
#include "includes/RenderComponent.hpp"

#include "includes/Engine.hpp"

int main() {
  Engine engine{};
  engine.startLoop();

  //   SceneManager sm{};
  //   sm.load();
  //   sm.run();
  //   sm.unload();

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
