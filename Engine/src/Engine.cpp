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

void Engine::load_test_scene() {
  _scene = std::make_unique<Scene>(this, "hewwo :3");
  _scene->generate_test_scene();
  auto new_s = std::make_unique<Scene>(this);
  change_scene(std::move(new_s));
  _scene->generate_test_scene();
}

bool Engine::read_scene_from_json(std::filesystem::path p) {
  auto s = BigJson{}.read_from_json(p, this);
  if (s.has_value()) {
    change_scene(std::move(s.value()));
  } else {
    LOG_ERROR("Scene could not be loaded");
    return false;
  }
  return true;
}

bool Engine::save_current_scene_as_json(std::filesystem::path p) {
  if (!BigJson{}.write_to_json(p, get_scene())) {
    LOG_ERROR("Scene could not be saved");
    return false;
  }
  return true;
}

void Engine::change_scene(std::unique_ptr<Scene> s) {
  // ensure active scene is destroyed first
  _scene = nullptr;

  _scene = std::move(s);
}

void Engine::startLoop() {
  LOG("Engine::startLoop()");

  // Time data to keep track of
  float total_time = 0.f, accumulated_time = 0.f;
  float current_time = get_total_time();
  // frame represent updates to the scene itself, which occur in the inner loop
  int frames = 0;
  // sub-frames represent each iteration of the outer loop
  int sub_frames = 0;

  float frame_time, new_time;
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

    // process ECS once per tick (_s_per_update = 1/FRAME_RATE_HZ)
    // note: accumulated_time is increasing by a large magnitude faster!
    while (accumulated_time >= _s_per_update) {
      frames++;
      total_time += _s_per_update;
      accumulated_time -= _s_per_update;

      // create snapshot here -> this can be used for extensive per frame
      // debugging!
      FrameSnapshot s(total_time, _s_per_update, accumulated_time, frames,
                      sub_frames);

      // scene should never be a nullptr!
      if (_scene != nullptr) {
        _scene->update(s);
        _wm.swap_buffers();
      }
      sub_frames = 0;

      Log::get_instance().clear_buffer();
    }
  }
}
} // namespace RT
