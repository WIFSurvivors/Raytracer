#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/tcp_server/TcpServer.hpp"
#include "includes/utility/Timer.hpp"
#include <iostream>

class TcpServer;
Engine::Engine() : _tcp_server(std::make_shared<TcpServer>(51234, this)) {
  init_server();
}
Engine::~Engine() { stop_server(); }

void Engine::init_server() {
  LOG("Engine::init_server()");
  try {
    _tcp_server->start();
  } catch (const std::exception &e) {
    LOG_ERROR("Error: " + std::string(e.what()));
  }
}

void Engine::stop_server() {
  LOG("Engine::stop_server()");
  try {
    _tcp_server->stop();
  } catch (const std::exception &e) {
    LOG_ERROR("Error: " + std::string(e.what()));
  }
}

void Engine::startLoop() {
  LOG("Engine::startLoop()");
  Timer timer;

  float time = 0.f;
  float current_time = timer.get_duration();
  float accumulator = 0.f;
  int frames = 0, sub_frames = 0;

  float frame_time, new_time;

  // #define FRAME_CAP (30.f)
  // constexpr float MS_PER_UPDATE = 1 / FRAME_CAP; // = 0.0333f
#if SHOW_UI
  while (_wm.should_close()) {
#else
  while (true) {
#endif

    new_time = timer.get_duration();
    frame_time = new_time - current_time;
    current_time = new_time;

    accumulator += frame_time;
    frames++;
    sub_frames = 0;

    // LOG(std::format("frames: {}", frames));
    // Log::get_instance().start_new_entry(count, frame_time, new_time);

    // process input (tcp counts as input :D)
    _tcp_server->execute_command();
    _wm.update_input();

    // makes sure, that minimum update rate is 30HZ (or whatever value you
    // entered via cmake) and not any faster!
    while (accumulator >= MS_PER_UPDATE) {
      //   LOG_ERROR(std::format("accumulator: {}", accumulator));
      accumulator -= MS_PER_UPDATE;
      _scene.update(MS_PER_UPDATE);
      time += MS_PER_UPDATE;
      sub_frames++;

      // update this ...
      _scene.render(new_time); // update the world with most recent data!
      _wm.swap_buffers();
    }
    // LOG_WARN(std::format("sub_frames: {}", sub_frames));

    // Log::get_instance().clear_buffer();
  }
}
