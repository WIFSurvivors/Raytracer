#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#define GET_LOG_PATH_COMMAND "GetLogPath"
struct GetLogPath : public TcpCommand
{
	public:
    GetLogPath() {}
    std::string execute(Engine * engine) override;
    std::string undo() override;
	private:
};

