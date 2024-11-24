#include "includes/Engine.hpp"

#include "includes/utility/SimpleLogger.hpp"

Engine::Engine() {
  SimpleLogger::print("engine 1");
  init();
  SimpleLogger::print("engine 2");
}

void Engine::startLoop() {}

UUIDManager *Engine::get_active_uuid_manager() {
  //   return _scene.get_uuid_manager();
  return nullptr;
}

void Engine::init() {}