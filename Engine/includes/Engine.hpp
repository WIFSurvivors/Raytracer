#pragma once

#include "includes/Scene.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/tcp_server/TcpServer.hpp"

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
  virtual ~Engine();

  void startLoop();

  inline UUIDManager *get_active_uuid_manager() {
    return _scene.get_uuid_manager();
  }
  inline WindowManager *get_window_manager() { return &_wm; }
  inline Scene *get_scene() { return &_scene; }
  inline RenderSystem *get_render_system() {
    return _scene.get_render_system();
  } // temporary solution
  inline void set_tcp_server_message_received(bool value) {
    _tcp_server_message_received = value;
  }

private:
  void init_server();
  void stop_server();
  WindowManager _wm{};
  Scene _scene{this}; // scene needs be initalized last
  std::shared_ptr<TcpServer> _tcp_server =
      std::make_shared<TcpServer>(51234, this);
  bool _tcp_server_message_received = false;
};
