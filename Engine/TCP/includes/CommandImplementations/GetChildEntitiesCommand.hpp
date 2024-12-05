#pragma once
#include "includes/TcpCommand.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>

#define GET_ENTITIES_COMMAND "GetChildEntities"

struct GetChildEntitiesCommand : public TcpCommand {
    GetChildEntitiesCommand(uuid_t uuid ) : TcpCommand(uuid) {};
    std::string execute(Engine *e) override;
    int undo() override;
    private:
};
