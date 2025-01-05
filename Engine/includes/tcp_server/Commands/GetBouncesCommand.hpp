#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define GET_BOUNCES_COMMAND "GetBounces"
class GetBouncesCommand : public TcpCommand
{
public:
    explicit GetBouncesCommand() {}
    std::string execute(Engine * engine) override;
    std::string undo() override;
private:
};
