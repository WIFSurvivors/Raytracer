#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define GET_BOUNCES_COMMAND "GetBounces"
class GetBouncesCommand : public TcpCommand {
public:
  GetBouncesCommand() {}
  std::string execute(RT::Engine *engine) override;

private:
};
