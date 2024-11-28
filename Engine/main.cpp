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
  Engine engine{};
  SimpleLogger::print("=== START LOOP ===");
  engine.startLoop();
  SimpleLogger::print("=== APP ENDED ===");
    return 0;
}
