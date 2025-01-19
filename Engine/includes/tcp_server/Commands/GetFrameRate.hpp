#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define GET_FRAME_RATE_COMMAND "GetFrameRate"
class GetFrameRate : public TcpCommand {
public:
  GetFrameRate() {}
  std::string execute(RT::Engine *engine) override;

private:
};
