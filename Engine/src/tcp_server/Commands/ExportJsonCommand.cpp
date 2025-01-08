#include "includes/tcp_server/Commands/ExportJsonCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

using namespace RT;

std::string ExportJsonCommand::execute(Engine *engine) {
  return "ExportJsonCommand executed but empty";
}

std::string ExportJsonCommand::undo() { throw NotImplementedError(); }
