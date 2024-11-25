#include "includes/system/RenderSystem.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/SimpleLogger.hpp"

int main() {
  SimpleLogger::print("=== APP STARTED ===");
  Engine engine{};
  SimpleLogger::print("=== START LOOP ===");
  engine.startLoop();
  SimpleLogger::print("=== APP ENDED ===");

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
