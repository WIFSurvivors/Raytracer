#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define GET_FOV_COMMAND "GetFov"
class GetFovCommand : public TcpCommand
{
public:
    explicit GetFovCommand() {}
    std::string execute(RT::Engine * engine) override;
    std::string undo() override;
private:

};
