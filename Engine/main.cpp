#include "includes/TcpServer.hpp"
#include "includes/Entity.hpp"
#include "includes/WindowManager.hpp"
#include "includes/system/RenderSystem.hpp"
#include "includes/component/RenderComponent.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include <memory>

int main() {
  SimpleLogger::print("=== APP STARTED ===");
  // Engine engine{};
  WindowManager wm{};
  RenderSystem rs{&wm};
  rs.init();
  RenderComponent comp;
  comp.init(rs.program->programID);
  rs._component = std::make_unique<RenderComponent>(comp);
  SimpleLogger::print("=== START LOOP ===");
  // engine.startLoop();
  rs.render();

  SimpleLogger::print("=== APP ENDED ===");
  comp.destroy();
  rs.destroy();
    /*


    core::RenderComponent comp;
    core::RenderComponent comp02;
    rs._component = std::make_unique<core::RenderComponent>(comp);
    rs.init();

    rs.render(); // shouldn't exist

    rs.destroy();
    */
    return 0;
}
