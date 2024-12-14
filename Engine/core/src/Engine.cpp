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
  SimpleLogger::print("Engine::startLoop()");
  float t0, t1, dt;
  #if SHOW_UI
  while (_wm.shouldClose()) {
  #else
  while (true) {
  #endif
    t0 = t1;
    t1 += 0.1f; // fixed time step! CAN BE WAY TOO FAST
    dt = t1 - t0;
    _scene.update(t1);
    _wm.update();
  }
}
