#include "includes/tcp_server/Commands/GetBouncesCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

using RT::Log;
using RT::Engine;

std::string GetBouncesCommand::execute(Engine *engine) {
  if (!engine) {
    LOG_ERROR("Engine is null");
    return "Engine is null";
  }
  auto *scene = engine->get_scene();
  if (!scene) {
    LOG_ERROR("Scene is null");
    return "Scene is null";
  }
  auto render_system = scene->get_render_system();
  if (!render_system) {
    LOG_ERROR("RenderSystem is null");
    return "RenderSystem is null";
  }
  return std::to_string(render_system->get_bounces());
}

