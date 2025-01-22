#include "includes/tcp_server/Commands/ImportJsonCommand.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <format>
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

using RT::Log;
using RT::Engine;

std::string ImportJsonCommand::execute(Engine *engine) {
  try {
    std::ifstream file(_json_path);
    if (!file.is_open()) {
      std::string error = "Failed to open file: " + _json_path;
      LOG_ERROR(error);
      return error;
    }
    file.close();
    engine->read_scene_from_json(_json_path);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return e.what();
  }
    set_successfull(true);
  return "Scene loaded";
}
