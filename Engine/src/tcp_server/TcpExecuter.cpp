#include "includes/tcp_server/TcpExecuter.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include <string>

TcpExecuter::TcpExecuter() {}

std::string TcpExecuter::execute(TcpCommand *command, Engine *engine) {
  std::string return_value = command->execute(engine);
  LOG_TCP("Returning to client: " + return_value);
  return return_value;
}

std::string TcpExecuter::undo(TcpCommand *command) { throw NotImplementedError(); }
