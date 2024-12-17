#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>

#define GET_COMPONENTS_COMMAND "GetComponents"

struct GetComponentsCommand : public TcpCommand {
    GetComponentsCommand(uuid__ uuid ) : TcpCommand(uuid) {};
    std::string execute(Engine *e) override;
    int undo() override;
    private:
};