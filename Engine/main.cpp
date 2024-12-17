#include "includes/tcp_server/TcpServer.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include <exception>
#include <string>

// #ifndef SHOW_UI
// #define SHOW_UI
// #endif

int main() {
#if SHOW_UI
  SimpleLogger::print("=== APP STARTED === UI ENABLED");
#else
  SimpleLogger::print("=== APP STARTED === UI DISABLED");
#endif
  try {
    Engine engine{};
    engine.startLoop();
  } catch (std::exception &e) {
    SimpleLogger::print("App crashed!!!!");
    SimpleLogger::print(std::string(e.what()));
  }
  SimpleLogger::print("=== APP ENDED ===");
  return 0;
}
