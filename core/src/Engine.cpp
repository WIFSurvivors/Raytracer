#include "includes/Engine.hpp"

Engine::Engine() { init(); }

void Engine::startLoop() {}

UUIDManager *Engine::get_uuid_manager() { return &_uuid_manager; }

void Engine::init() {}