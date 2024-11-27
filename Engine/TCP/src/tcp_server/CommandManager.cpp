#include "includes/CommandManager.hpp"


void CommandManager::push(std::unique_ptr<TcpCommand> command) {
  _command_queue.push(std::move(command));
}

std::unique_ptr<TcpCommand> CommandManager::pop() {
  std::unique_ptr<TcpCommand> command = std::move(_command_queue.front());
  _command_queue.pop();
  return std::move(command);
}

std::string CommandManager::execute_command(std::string command) {
  std::unique_ptr<ParsedTcpCommand> parsed_command = _parser.parse(command);
  std::unique_ptr<TcpCommand> tcp_command =
      _factory.create_command(*parsed_command);
  if (tcp_command) {
    int return_value = _executer.execute(tcp_command.get(), _engine);
    if(return_value == 0){
      push(std::move(tcp_command));
    }
    return std::to_string(return_value);
  } else {
    std::cout << "Failed to parse command" << std::endl;
    return "Failed to parse command";
  }
}
