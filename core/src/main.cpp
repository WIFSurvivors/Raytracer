#include "includes/RenderSystem.hpp"
#include "includes/RenderComponent.hpp"

#include "includes/Engine.hpp"

#include "includes/utility/SimpleLogger.hpp"

int main() {
  SimpleLogger::print("main 1");
  Engine engine{};
  SimpleLogger::print("main 2");
  engine.startLoop();
  SimpleLogger::print("main 3");

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
