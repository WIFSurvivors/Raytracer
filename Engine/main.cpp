#include "includes/Engine.hpp"
#include "includes/tcp_server/TcpServer.hpp"
#include "includes/utility/Log.hpp"
#include <exception>
#include <string>
#include <format>

/*
 * Initializes Log system and then start Engine, which will take control for the
 * rest of the life time.
 */
int main() {
  using namespace RT;
  Log::get_instance().set_cout_log_level(Log::Level::Tcp);
  Log::get_instance().set_file_log_level(Log::Level::Tcp);
  Log::get_instance().init_file();
  Log::get_instance().clear_buffer();

#if SHOW_UI
  LOG(std::format("=== APP STARTED | UI=ON === FRAME RATE {}", FRAME_RATE_HZ));
#else
  LOG("=== APP STARTED | UI=OFF ===")
#endif
  try {
	// Create Engine, load default scene and then start main loop
    RT::Engine engine{};
	engine.read_scene_from_json("./json/JsonDefault.json");
    engine.startLoop();
  } catch (std::exception &e) {
    LOG_ERROR("App crashed!!!!");
    LOG_ERROR(std::string(e.what()));
  }
  LOG("=== APP ENDED ===");
  Log::get_instance().clear_buffer();
  return 0;
}
