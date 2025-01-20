#pragma once
#include <string>
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/Engine.hpp"
using RT::Engine;
struct TcpExecuter {
  TcpExecuter();
  /// @brief Execute a command.
  /// The command is executed by calling the execute function of the command.
  /// The command is executed with the engine as a parameter. 
  /// @param command The command to execute.
  /// @param engine The engine that the command is executed on.
  /// @return The return message of the command.
  std::string execute(TcpCommand *command, RT::Engine *engine);
  /// @brief Undo a command.
  /// The command is undone by calling the undo function of the undoable command.
  /// The command is undone with the engine as a parameter.
  /// @param command The command to undo.
  /// @param engine The engine that the command is undone on.
  /// @return The return message of the command.
  std::string undo(UndoableCommand *command, RT::Engine *engine);

private:
};
