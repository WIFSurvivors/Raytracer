#pragma once
#include <map>
#include <string>
#include <vector>
#include <boost/uuid/uuid.hpp>

#include <memory>
#include <cstdint>
#include <iostream>

class Engine;

struct TcpCommand {
  using uuid = boost::uuids::uuid;
  
  std::vector<std::string> parameters;
  TcpCommand() = default;
  explicit TcpCommand(uuid id) : _uuid(id) {}
  virtual ~TcpCommand() = default;
  virtual std::string execute(Engine *engine) = 0;
  virtual int undo() = 0;
  uuid _uuid;

private:
  std::map<std::string, std::string> _previous_state;
};
