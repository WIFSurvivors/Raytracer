#include "includes/tcp_server/TcpServer.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include <exception>
#include <string>

// #ifndef SHOW_UI
// #define SHOW_UI
// #endif

int main() {
  Log::set_log_level(Log::Level::Message);
#if SHOW_UI
  Log::message("=== APP STARTED === UI=ON");
#else
  Log::message("=== APP STARTED === UI=OFF");
#endif
  try {
    Engine engine{};
    engine.startLoop();
  } catch (std::exception &e) {
    Log::error("App crashed!!!!");
    Log::error(std::string(e.what()));
  }
  Log::message("=== APP ENDED ===");
  return 0;
}
