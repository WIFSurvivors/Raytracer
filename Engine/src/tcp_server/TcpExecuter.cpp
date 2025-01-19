#include "includes/tcp_server/TcpExecuter.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include <string>

using RT::Log;
using RT::Engine;

TcpExecuter::TcpExecuter() {}

std::string TcpExecuter::execute(TcpCommand *command, Engine *engine) {
  std::string return_value = command->execute(engine);
  LOG_TCP("Returning to client: " + return_value);
  return return_value;
}

std::string TcpExecuter::undo(UndoableCommand *command, Engine *engine) {
  if (command == nullptr) {
    LOG_TCP("Undo command is null");
    return "Error: Undo command is null";
  }
  return command->undo(engine);
}
