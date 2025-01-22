#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/tcp_server/TcpParser.hpp"
#include "includes/tcp_server/TcpExecuter.hpp"
#include "includes/tcp_server/TcpCommandFactory.hpp"
#include "includes/tcp_server/ParsedTcpCommand.hpp"
// #include "includes/Engine.hpp"
#include <queue>
#include <memory>
#include <iostream>
#include <string>
#include <mutex>
#include <stack>

class TcpServer;

using RT::Engine;

struct CommandManager {
  /// @brief Constructor for the command manager.
  /// @param tcp_server  The tcp server that the command manager is connected to.
  /// @param engine  The engine that the commands are executed on.
  CommandManager(TcpServer *tcp_server, RT::Engine *engine)
      : _tcp_server(tcp_server), _engine(engine),
        _command_queue(std::queue<std::unique_ptr<TcpCommand>>()) {}
  /// @brief Push a command to the command queue.
  /// @param command The command to push to the queue.
  void push(std::unique_ptr<TcpCommand> command);
  /// @brief Pop a command from the command queue.
  /// @return The command that was popped.
  std::unique_ptr<TcpCommand> pop();
  /// @brief Execute a command.
  /// The command is executed by aggregating the command to the TcpExecuter.
  /// @param command The command to execute.
  /// @return The return message of the command.
  void execute_command();
  /// @brief Execute all commands in the command queue.
  /// The commands in the queue are executed by calling the execute_command function.
  void execute_commands();
  /// @brief Decode a command from a string.
  /// The command is decoded by parsing the command string.
  /// @param command The command string to decode.
  /// @return The parsed command.
  inline auto decode_command(std::string command) {
    return _parser.parse(command);
  }
  /// @brief Create a command from a parsed command.
  /// The command is created by aggregating the parsed command to the TcpCommandFactory.
  /// @param parsed_command The parsed command to create the command from.
  /// @return The created command.
  auto inline create_command(ParsedTcpCommand parsed_command) {
    return _factory.create_command(parsed_command);
  }
  /// @brief Undo commands.
  /// The command is undone by calling the undo function of the undoable command.
  /// @param number The number of commands to undo.
  void undo_command(int number);
  /// @brief Destructor for the command manager.
  /// The destructor clears the command queue and the undo stack.
  ~CommandManager() {
    while (!_command_queue.empty()) {
      _command_queue.pop();
    }
    while (!_undo_stack.empty()) {
      _undo_stack.pop();
    }
  }

private:
  std::queue<std::unique_ptr<TcpCommand>> _command_queue{};
  std::stack<std::unique_ptr<UndoableCommand>> _undo_stack{};
  TcpParser _parser{};
  TcpCommandFactory _factory{};
  TcpExecuter _executer{};
  RT::Engine *_engine;
  TcpServer *_tcp_server;
};
