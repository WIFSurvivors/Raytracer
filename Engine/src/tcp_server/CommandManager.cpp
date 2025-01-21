#include "includes/tcp_server/CommandManager.hpp"
#include "includes/tcp_server/TcpServer.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include "includes/tcp_server/Commands/UndoCommand.hpp"
#include "includes/tcp_server/Commands/RemoveEntity.hpp"
#include "includes/tcp_server/Commands/ImportJsonCommand.hpp"
#include "includes/tcp_server/Commands/RemoveComponent.hpp"
#include <format>

using RT::Log;
void CommandManager::execute_commands() { 
  // Is currently not used in the project but could be used to execute all commands in the queue, if needed.
  while (!_command_queue.empty()) {
    execute_command();
  }
 }

void CommandManager::execute_command() {
  auto command = pop();
  if (command == nullptr) {
    return;
  }
  // Check if the command is an undo command
  if(auto undo_c = dynamic_cast<UndoCommand*>(command.get())) {
    // Undo the number of commands specified in the undo command 
    undo_command(undo_c->get_number());
    return;
  }
  auto msg = _executer.execute(command.get(), _engine);
  // Check if the command was successfull
  if(!command->is_successfull()) {
    _tcp_server->send_message("- ERROR - " + msg);
    return;
  }
  // If the command is a remove command or the scene is changed, clear the undo stack for consistency
  if(dynamic_cast<RemoveComponent*>(command.get()) || dynamic_cast<RemoveEntity*>(command.get()) || dynamic_cast<ImportJsonCommand*>(command.get())) {
    while(!_undo_stack.empty()) {
      _undo_stack.pop();
    }
  }

  // Check if the command is undoable and push it to the undo stack
  if (auto undoable_command = dynamic_cast<UndoableCommand*>(command.get())) {
    _undo_stack.push(std::unique_ptr<UndoableCommand>(undoable_command));
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
  if(_undo_stack.empty()) {
    _tcp_server->send_message("No commands to undo");
    return;
  }
  for (int i = 0; i < number; i++) {
    auto command = std::move(_undo_stack.top());
    auto msg = _executer.undo(std::move(command.get()), _engine);
    _undo_stack.pop();
  }
    _tcp_server->send_message("Undoing " + std::to_string(number) + " commands");
}

std::unique_ptr<TcpCommand> CommandManager::pop() {
  if (_command_queue.empty()) {
    return nullptr;
  }
  std::unique_ptr<TcpCommand> command = std::move(_command_queue.front());
  _command_queue.pop();
  return std::move(command);
}
