#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>

#define GET_ENTITIY_OPTIONS_COMMAND "GetEntityOptions"

struct GetEntityOptionsCommand : public TcpCommand {
    explicit GetEntityOptionsCommand(uuid__ uuid ) : TcpCommand(uuid) {}
    std::string execute(Engine *e) override;
    int undo() override;
    private:
};
