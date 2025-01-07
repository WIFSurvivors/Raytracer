#include "includes/tcp_server/TcpServer.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include <exception>
#include <string>

int main() {
  Log::get_instance().set_cout_log_level(Log::Level::Tcp);
  Log::get_instance().set_file_log_level(Log::Level::Tcp);
  Log::get_instance().display_color_demo();
  Log::get_instance().init_file();
  Log::get_instance().clear_buffer();

#if SHOW_UI
  LOG("=== APP STARTED | UI=ON ===")
#else
  LOG("=== APP STARTED | UI=OFF ===")
#endif
  try {
    Engine engine{};
	engine.save_scene_as_json("wawa.txt");
    engine.startLoop();
  } catch (std::exception &e) {
	LOG_ERROR("App crashed!!!!");
    LOG_ERROR(std::string(e.what()));
  }
  LOG("=== APP ENDED ===");
  Log::get_instance().clear_buffer();
  return 0;
}
