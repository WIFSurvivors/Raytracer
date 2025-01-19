#include "includes/tcp_server/Commands/GetFrameRate.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

using RT::Log;
using RT::Engine;

std::string GetFrameRate::execute(Engine *engine) {
  if (!engine) {
    LOG_ERROR("Engine is null");
    return "Engine is null";
  }
  return std::to_string(static_cast<int>(engine->get_frame_rate()));
}

