#include "includes/Engine.hpp"

#include "includes/utility/SimpleLogger.hpp"

Engine::Engine() { init(); }

void Engine::init() {}

void Engine::startLoop() {}

UUIDManager *Engine::get_active_uuid_manager() {
  return _scene.get_uuid_manager();
}