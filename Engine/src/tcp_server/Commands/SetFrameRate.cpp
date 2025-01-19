#include "includes/tcp_server/Commands/SetFrameRate.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

using RT::Engine;
using RT::Log;

std::string SetFrameRate::execute(Engine *engine) {
  if (!engine) {
    LOG_ERROR("Engine is null");
    return "Engine is null";
  }
  _old_frame_rate = engine->get_frame_rate();
    engine->set_frame_rate(_frame_rate);
  return "FrameRate set to " + std::to_string(static_cast<int>(_old_frame_rate));
}

std::string SetFrameRate::undo(RT::Engine *engine) { 
    if (!engine) {
    LOG_ERROR("Engine is null");
    return "Engine is null";
  }
    engine->set_frame_rate(_old_frame_rate);
  return "FrameRate set to " + std::to_string(static_cast<int>(_old_frame_rate));
  
 }
