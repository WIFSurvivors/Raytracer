#include "includes/Engine.hpp"
#include "includes/utility/SimpleLogger.hpp"

class TcpServer;
Engine::Engine() { init(); }

void Engine::init() {
  SimpleLogger::print("Engine::init()");
  try {
    _tcp_server->start();
  } catch (const std::exception &e) {
    SimpleLogger::print("Error: " + std::string(e.what()));
  }
  _render_system.init();
}

void Engine::startLoop() {
  RenderComponent comp{get_active_uuid_manager()->getNewUUID(), _scene.get_root().lock().get(), _render_system.program->programID};
  comp.init(_render_system.program->programID);
  _render_system._component = std::make_unique<RenderComponent>(comp);
  _render_system.render();
  comp.destroy();
  _render_system.destroy();
}

UUIDManager *Engine::get_active_uuid_manager() {
  return _scene.get_uuid_manager();
}
