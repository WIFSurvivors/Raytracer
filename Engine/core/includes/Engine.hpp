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

  UUIDManager *get_active_uuid_manager();
  Scene *get_scene() { return &_scene; }
private:
  void init();
  WindowManager _wm{};
  // this should potentially be safed here or in main(). it'll depend on how to
  // this is integrated into the C# GUI
  Scene _scene{this}; // scene should be initalized last
  std::shared_ptr<TcpServer> _tcp_server = std::make_shared<TcpServer>(51234, this);
  RenderSystem _render_system{&_wm};
};
