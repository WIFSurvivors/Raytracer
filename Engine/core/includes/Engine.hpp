#pragma once

#include "includes/Scene.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/TcpServer.hpp"

#include <memory>

/**
 * Engine is responsible for starting the app and the fundamental parts.
 * These are the UUID Manager, a simple Scene (which initializes the  ECS) and
 * the TCP Server (and potentially the Window Manager). After these are
 * initialized, the rendering and data communication can start.
 */
class Engine {
public:
  Engine();
  virtual ~Engine() = default;

  void startLoop();

  inline UUIDManager *get_active_uuid_manager() {
    return _scene.get_uuid_manager();
  }
  inline WindowManager *get_window_manager() { return &_wm; }
  inline Scene *get_scene() { return &_scene; }

private:
  void init();
  WindowManager _wm{};
  std::shared_ptr<TcpServer> _tcp_server =
      std::make_shared<TcpServer>(51234, this);
  Scene _scene{this}; // scene needs be initalized last
};
