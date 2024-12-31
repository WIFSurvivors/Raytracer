#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>

#define GET_COMPONENT_OPTIONS_COMMAND "GetComponentOptions"

struct GetComponentOptions : public TcpCommand {
    explicit GetComponentOptions(uuid__ uuid ) : TcpCommand(uuid) {}
    std::string execute(Engine *e) override;
    int undo() override;
    private:
};
