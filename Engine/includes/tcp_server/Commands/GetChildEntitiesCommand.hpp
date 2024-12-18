#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>

#define GET_ENTITIES_COMMAND "GetChildEntities"

struct GetChildEntitiesCommand : public TcpCommand {
    explicit GetChildEntitiesCommand(uuid__ uuid ) : TcpCommand(uuid) {}
    std::string execute(Engine *e) override;
    int undo() override;
    private:
};
