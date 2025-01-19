#pragma once
#include <map>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include "includes/Engine.hpp"
#include <vector>

using RT::Engine;

struct TcpCommand {
  using uuid = boost::uuids::uuid;

  std::vector<std::string> parameters;
  TcpCommand() = default;
  explicit TcpCommand(uuid id) : _uuid(id) {}
  virtual ~TcpCommand() = default;
  virtual std::string execute(RT::Engine *engine) = 0;
  inline uuid get_uuid() const { return _uuid; }
  inline void set_uuid(uuid id) { _uuid = id; }
  inline bool is_successfull() const { return _successfull; }
  inline void set_successfull(bool success) { _successfull = success; }

private:
  bool _successfull = false;
  uuid _uuid;
  std::map<std::string, std::string> _previous_state;
};

struct UndoableCommand : public TcpCommand {
  using TcpCommand::TcpCommand; 

  virtual std::string undo(RT::Engine *engine) = 0;

};

