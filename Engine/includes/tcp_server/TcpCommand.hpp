#pragma once
#include <map>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include "includes/Engine.hpp"
#include <vector>

using RT::Engine;
/// @brief Command that can be executed.
/// The execute function can be called to execute the command.
struct TcpCommand {
  using uuid = boost::uuids::uuid;

  std::vector<std::string> parameters;
  TcpCommand() = default;
  /// @brief Constructor for the command.
  /// @param id The uuid of the command, can be null.
  explicit TcpCommand(uuid id) : _uuid(id) {}
  virtual ~TcpCommand() = default;
  /// @brief Execute the command.
  /// The command is executed with the engine as a parameter.
  /// @param engine The engine that the command is executed on.
  /// @return The return message of the command.
  virtual std::string execute(RT::Engine *engine) = 0;
  inline uuid get_uuid() const { return _uuid; }
  inline void set_uuid(uuid id) { _uuid = id; }
  inline bool is_successfull() const { return _successfull; }
  inline void set_successfull(bool success) { _successfull = success; }

private:
  bool _successfull = false;
  uuid _uuid;
};
/// @brief Command that can be undone.
/// The undo function can be called to undo the command.
struct UndoableCommand : public TcpCommand {
  using TcpCommand::TcpCommand; 

  virtual std::string undo(RT::Engine *engine) = 0;

};

