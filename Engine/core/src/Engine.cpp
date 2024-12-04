#include "includes/Engine.hpp"
#include "includes/utility/SimpleLogger.hpp"

class TcpServer;
Engine::Engine() { init_server(); }

void Engine::init_server() {
  SimpleLogger::print("Engine::init()");
  try {
    _tcp_server->start();
  } catch (const std::exception &e) {
    SimpleLogger::print("Error: " + std::string(e.what()));
  }
}

void Engine::startLoop() {
  float t0, t1, dt;
  while (_wm.shouldClose()) {
    t0 = t1;
    t1 = _wm.get_time();
    dt = t1 - t0;
    _scene.update(t1);
    _wm.update();
  }
}
