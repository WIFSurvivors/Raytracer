#include "includes/tcp_server/TcpExecuter.hpp"
#include "includes/utility/Log.hpp"
#include <string>

TcpExecuter::TcpExecuter() {}

std::string TcpExecuter::execute(TcpCommand *command, Engine *engine) {
  std::string return_value = command->execute(engine);
  LOG_TCP("Returning to client: " + return_value);
  return return_value;
}

int TcpExecuter::undo(TcpCommand *command) { return 0; }
