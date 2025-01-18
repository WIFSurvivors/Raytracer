#include "includes/tcp_server/CommandManager.hpp"
#include "includes/tcp_server/TcpServer.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include "includes/tcp_server/Commands/UndoCommand.hpp"
#include <format>

using RT::Log;

void CommandManager::execute_commands() { throw NotImplementedError(); }

void CommandManager::execute_command() {
  auto command = pop();
  if (command == nullptr) {
    return;
  }
  if(auto undo_c = dynamic_cast<UndoCommand*>(command.get())) { 
    undo_command(undo_c->get_number());
    return;
  }
  auto msg = _executer.execute(command.get(), _engine);
  auto undoable_command = dynamic_cast<UndoableCommand*>(command.get());
  if (undoable_command) {
    _undo_queue.push(std::unique_ptr<UndoableCommand>(undoable_command));
    command.release();
  }
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

void CommandManager::undo_command(int number) {
  for (int i = 0; i < number; i++) {
    auto command = std::move(_undo_queue.front());
    auto msg = _executer.undo(std::move(command.get()), _engine);
    _tcp_server->send_message(msg);
    _undo_queue.pop();
  }
}

std::unique_ptr<TcpCommand> CommandManager::pop() {
  if (_command_queue.empty()) {
    return nullptr;
  }
  std::unique_ptr<TcpCommand> command = std::move(_command_queue.front());
  _command_queue.pop();
  return std::move(command);
}
