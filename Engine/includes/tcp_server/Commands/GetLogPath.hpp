#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define GET_LOG_PATH_COMMAND "GetLogPath"

struct GetLogPath : public TcpCommand {
  GetLogPath() {}
  std::string execute(RT::Engine *engine) override;

private:
};
