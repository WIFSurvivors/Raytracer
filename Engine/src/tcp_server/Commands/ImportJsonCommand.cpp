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

std::string importJsonCommand::execute(Engine *engine) {
  // TODO: Implement this method to read the json file with quicktype generated
  // code
  FILE *file = fopen(_json_path.c_str(), "r");
  if (!file) {
    std::string msg = std::format("File not found: {}", _json_path);
    LOG_ERROR(msg);
    return msg;
  }
  while (!feof(file)) {
    char c = fgetc(file);
    std::cout << c;
  }
  fclose(file);
  return "Imported json file";
}
int importJsonCommand::undo() { throw NotImplementedError{}; }
