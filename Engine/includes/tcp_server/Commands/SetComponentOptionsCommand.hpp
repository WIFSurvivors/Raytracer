#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <boost/json.hpp>

#define SET_COMPONENT_OPTIONS_COMMAND "SetComponentOptions"

struct SetComponentOptions : public TcpCommand {
  explicit SetComponentOptions(uuid uuid, boost::json::object obj)
      : TcpCommand(uuid), _obj(obj) {}
  std::string execute(RT::Engine *e) override;
  std::string undo() override;
  inline boost::json::object get_obj() const { return _obj; }
  inline void set_obj(boost::json::object obj) { _obj = obj; }

private:
  boost::json::object _obj;
};
