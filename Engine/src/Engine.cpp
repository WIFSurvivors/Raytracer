#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/tcp_server/TcpServer.hpp"
#include "includes/utility/FrameSnapshot.hpp"
#include "includes/utility/BigJsonReader.hpp"
#include <algorithm>
#include <iostream>
#include <memory>

class TcpServer;

namespace RT {
Engine::Engine() : _tcp_server(std::make_shared<TcpServer>(51234, this)) {
  init_server();
  _start_time = std::chrono::high_resolution_clock::now();
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

void Engine::read_scene_from_json(std::filesystem::path p) {
  //,std::unique_ptr<Scene> s
  BigJsonReader j;
  auto s = j.read_from_json(p, this);
  if (s.has_value()) {
    change_scene(std::move(s.value()));
  } else {
    LOG_ERROR("Scene could not be loaded");
    throw std::runtime_error("Scene could not be loaded");
  }
}

void Engine::save_scene_as_json(std::filesystem::path p) {
  BigJsonReader j;
  // j.read();

  // auto wawa = j.to_json(_scene);
  // std::cout << "RETURN:\n" << wawa << std::endl;
}

void Engine::change_scene(std::unique_ptr<Scene> s) { _scene = std::move(s); }

void Engine::startLoop() {
  LOG("Engine::startLoop()");
  float total_time = 0.f, accumulated_time = 0.f;
  float current_time = get_total_time();

  // frame represent updates to the scene itself, which occur in the inner loop
  int frames = 0;

  // sub-frames represent each iteration of the outer loop
  int sub_frames = 0;

  float frame_time, new_time;
  bool _temp = true;
  while (!_wm.should_close()) {
    // update the difference of the previous and the new frame
    new_time = get_total_time();
    frame_time = new_time - current_time;
    accumulated_time += frame_time;
    current_time = new_time;
    sub_frames++;

    // process input
    _tcp_server->execute_command();
    _wm.update_input();

    // process ECS once per tick (FRAME_RATE_HZ)
    while (accumulated_time >= MS_PER_UPDATE) {
      frames++;
      total_time += MS_PER_UPDATE;
      accumulated_time -= MS_PER_UPDATE;

      // create snapshot here
      FrameSnapshot s(total_time, MS_PER_UPDATE, accumulated_time, frames,
                      sub_frames);
      _scene->update(s);
      _wm.swap_buffers();
      sub_frames = 0;

      Log::get_instance().clear_buffer();
      // trigger this either on 1sec difference OR 10 log entries available???
    }

    // emulate change scene after 10 frames
    if (_temp && frames > 60) { // 2 seconds
      LOG("Engine::startLoop() THRESHHOLD GOT");
      _temp = false;
	  _scene->print_system_data();	  
      auto new_s = std::make_unique<Scene>(this, "test :3");
      change_scene(std::move(new_s));
      _scene->generate_test();
	  _scene->print_system_data();
    }
  }
}
} // namespace RT
