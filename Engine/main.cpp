#include "includes/TcpServer.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/SimpleLogger.hpp"

// #ifndef SHOW_UI
// #define SHOW_UI
// #endif

int main() {
  SimpleLogger::print("=== APP STARTED ===");
  Engine engine{};
  engine.startLoop();
  SimpleLogger::print("=== APP ENDED ===");
  return 0;
}
