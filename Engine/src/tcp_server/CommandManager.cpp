#include "includes/tcp_server/CommandManager.hpp"
#include "includes/tcp_server/TcpServer.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include <format>

using namespace RT;

void CommandManager::execute_commands() { throw NotImplementedError(); }

void CommandManager::execute_command() {
  auto command = pop();
  if (command == nullptr) {
    return;
  }
  auto msg = _executer.execute(command.get(), _engine);
  LOG_TCP(std::format("Command from queue received. Currently in Queue: {0}",
                      _command_queue.size()));
  if (msg.compare("0") != 0) {
    LOG("Command executed: " + msg);
  }
  _tcp_server->send_message(msg);
}

void CommandManager::push(std::unique_ptr<TcpCommand> command) {
  _command_queue.push(std::move(command));
}

std::unique_ptr<TcpCommand> CommandManager::pop() {
  if (_command_queue.empty()) {
    return nullptr;
  }
  std::unique_ptr<TcpCommand> command = std::move(_command_queue.front());
  _command_queue.pop();
  return std::move(command);
}
