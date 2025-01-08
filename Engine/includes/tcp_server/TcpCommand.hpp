#pragma once
#include <map>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <vector>

namespace RT {
struct Engine;
}

struct TcpCommand {
  using uuid = boost::uuids::uuid;

  std::vector<std::string> parameters;
  TcpCommand() = default;
  explicit TcpCommand(uuid id) : _uuid(id) {}
  virtual ~TcpCommand() = default;
  virtual std::string execute(RT::Engine *engine) = 0;
  virtual std::string undo() = 0;
  inline uuid get_uuid() const { return _uuid; }
  inline void set_uuid(uuid id) { _uuid = id; }

private:
  uuid _uuid;
  std::map<std::string, std::string> _previous_state;
};
