#include "includes/tcp_server/TcpServer.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include <exception>
#include <string>

// #ifndef SHOW_UI
// #define SHOW_UI
// #endif

int main() {
  auto l = Log::get_instance();
  l.set_log_level(Log::Level::Tcp);
  l.display_color_demo();
#if SHOW_UI
  LOG("=== APP STARTED | UI=ON ===")
#else
  LOG("=== APP STARTED | UI=OFF ===")
#endif
  try {
    Engine engine{};
    engine.startLoop();
  } catch (std::exception &e) {
	LOG_ERROR("App crashed!!!!");
    LOG_ERROR(std::string(e.what()));
  }
  LOG("=== APP ENDED ===");
  l.clear_buffer();
  return 0;
}
