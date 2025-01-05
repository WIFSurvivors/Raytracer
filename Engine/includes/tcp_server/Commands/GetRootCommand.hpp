#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#define GET_ROOT_COMMAND "GetRoot"
struct GetRootCommand : public TcpCommand
{
	GetRootCommand() = default; 
	std::string execute(Engine * e) override;
	std::string undo() override;

private:

};
