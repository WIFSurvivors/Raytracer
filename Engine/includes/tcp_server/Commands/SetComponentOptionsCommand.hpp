#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/json.hpp>

#define SET_COMPONENT_OPTIONS_COMMAND "SetComponentOptions"

struct SetComponentOptions : public TcpCommand {
    explicit SetComponentOptions(uuid uuid, boost::json::object obj) : TcpCommand(uuid), _obj(obj) {}
    std::string execute(Engine *e) override;
    int undo() override;
    private:
	boost::json::object _obj;
};
