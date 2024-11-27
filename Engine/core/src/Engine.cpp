#include "includes/Engine.hpp"

#include "includes/utility/SimpleLogger.hpp"

Engine::Engine() { init(); }

void Engine::init() {
  SimpleLogger::print("Engine::init()");
  try {
    _tcp_server->start();
  } catch (const std::exception &e) {
    SimpleLogger::print("Error: " + std::string(e.what()));
  }
}

void Engine::startLoop() {while (true)
{
  /* code */
}
}

UUIDManager *Engine::get_active_uuid_manager() {
  return _scene.get_uuid_manager();
}