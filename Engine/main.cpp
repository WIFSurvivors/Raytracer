#include "includes/tcp_server/TcpServer.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/SimpleLogger.hpp"

// #ifndef SHOW_UI
// #define SHOW_UI
// #endif

int main() {
#if SHOW_UI
  SimpleLogger::print("=== APP STARTED === UI ENABLED");
#else
  SimpleLogger::print("=== APP STARTED === UI DISABLED");
#endif
  Engine engine{};
  engine.startLoop();
  SimpleLogger::print("=== APP ENDED ===");
  return 0;
}
