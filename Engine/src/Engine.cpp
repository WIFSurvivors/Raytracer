#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include <iostream>
class TcpServer;
Engine::Engine() { init_server(); }
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
  // this->get_scene()->get_entity_storage()->print();
  float t0, t1, dt;
#if SHOW_UI
  while (_wm.shouldClose()) {
#else
  while (true) {
#endif
    while (_tcp_server_message_received) {
    }
    t0 = t1;
    t1 += 0.1f; // fixed time step! CAN BE WAY TOO FAST
    dt = t1 - t0;
    _scene.update(t1);
    _wm.update();
  }
}
