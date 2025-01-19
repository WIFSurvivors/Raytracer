#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <boost/json.hpp>

#define SET_COMPONENT_OPTIONS_COMMAND "SetComponentOptions"

struct SetComponentOptions : public UndoableCommand {
  explicit SetComponentOptions(uuid uuid, boost::json::object obj)
      : UndoableCommand(uuid), _obj(obj) {}
  std::string execute(RT::Engine *e) override;
  std::string undo(RT::Engine *e) override;
  inline boost::json::object get_obj() const { return _obj; }
  inline void set_obj(boost::json::object obj) { _obj = obj; }
  inline boost::json::object get_old_obj() const { return _old_obj; }

private:
  boost::json::object _obj;
  boost::json::object _old_obj;
};
