#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/tcp_server/TcpServer.hpp"
#include <iostream>
class TcpServer;
Engine::Engine() : _tcp_server(std::make_shared<TcpServer>(51234, this)) {
  init_server();
}
Engine::~Engine() { stop_server(); }

void Engine::init_server() {
  Log::message("Engine::init_server()");
  try {
    _tcp_server->start();
  } catch (const std::exception &e) {
    Log::message("Error: " + std::string(e.what()));
  }
}

void Engine::stop_server() {
  Log::message("Engine::stop_server()");
  try {
    _tcp_server->stop();
  } catch (const std::exception &e) {
    Log::error("Error: " + std::string(e.what()));
  }
}

void Engine::startLoop() {
  Log::message("Engine::startLoop()");
  std::cout << std::endl;
  float t0, t1, dt;
#if SHOW_UI
  while (_wm.shouldClose()) {
#else
  while (true) {
#endif
    _tcp_server->execute_command();
    t0 = t1;
#if SHOW_UI
    t1 = _wm.get_time();
#else
    t1 += 0.1f; // fixed time step! CAN BE WAY TOO FAST
#endif
    dt = t1 - t0;
    _scene.update(t1);
    _wm.update();
  }
}
