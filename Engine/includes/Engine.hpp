#pragma once

#include "includes/Scene.hpp"
#include "includes/UUIDManager.hpp"

#include <filesystem>
#include <memory>
#include <mutex>
#include <chrono>

#ifndef FRAME_RATE_HZ // can be set in cmake
#define FRAME_RATE_HZ (30)
#endif
constexpr float MS_PER_UPDATE = 1.f / FRAME_RATE_HZ;

class TcpServer;

/**
 * Engine is responsible for starting the app and the fundamental parts.
 * These are the UUID Manager, a simple Scene (which initializes the  ECS) and
 * the TCP Server (and potentially the Window Manager). After these are
 * initialized, the rendering and data communication can start.
 * Additionally. it runs a timer which caps the app to a specified frame limit.
 */
namespace RT {
class Engine {
public:
  Engine();
  virtual ~Engine();

  void load_sample_scene() {
    _scene = std::make_unique<Scene>(this);
    _scene->generate_sample_content();
  }
  void save_scene_as_json(std::filesystem::path p);
  void read_scene_from_json(std::filesystem::path p);
  void startLoop();

  inline UUIDManager *get_active_uuid_manager() {
    return _scene->get_uuid_manager();
  }
  inline WindowManager *get_window_manager() { return &_wm; }
  inline Scene *get_scene() { return _scene.get(); }
  // inline RenderSystem *get_render_system() {
  //   return _scene->get_render_system();
  // } // temporary solution
  inline std::mutex *get_ecs_mutex() { return &_ecs_mutex; }

  inline float get_total_time() const {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<float>(now - _start_time).count();
  }

private:
  void init_server();
  void stop_server();

  void change_scene(std::unique_ptr<Scene> s);

  std::mutex _ecs_mutex;
  WindowManager _wm{};
  std::unique_ptr<Scene> _scene;
  // Scene _scene{this}; // scene needs be initalized last
  std::shared_ptr<TcpServer> _tcp_server;

  std::chrono::high_resolution_clock::time_point _start_time;
};
} // namespace RT
