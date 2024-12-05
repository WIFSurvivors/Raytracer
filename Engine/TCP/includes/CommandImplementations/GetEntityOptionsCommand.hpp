#pragma once
#include "includes/TcpCommand.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>

#define GET_ENTITIY_OPTIONS_COMMAND "GetEntityOptions"

struct GetEntityOptionsCommand : public TcpCommand {
    GetEntityOptionsCommand(uuid_t uuid ) : TcpCommand(uuid) {};
    std::string execute(Engine *e) override;
    int undo() override;
    private:
};