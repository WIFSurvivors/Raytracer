#pragma once
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include <boost/uuid/uuid.hpp>
class Engine;
typedef boost::uuids::uuid uuid_t;
struct TcpCommand {
  std::vector<std::string> parameters;
  TcpCommand() = default;
  explicit TcpCommand(uuid_t uuid) : _uuid(uuid) {}
  virtual ~TcpCommand() = default;
  virtual std::string execute(Engine * engine) = 0;
  virtual int undo() = 0;
    uuid_t _uuid;
  private:
    std::map<std::string, std::string> _previous_state;
};
