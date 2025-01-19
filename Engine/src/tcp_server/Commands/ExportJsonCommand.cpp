#include "includes/tcp_server/Commands/ExportJsonCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

using RT::Log;
using RT::Engine;

std::string ExportJsonCommand::execute(Engine *engine) {
  try {
    engine->save_scene_as_json(_json_path);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return e.what();
  }
  set_successfull(true);
  return "Scene exported on path: " + _json_path;
}

