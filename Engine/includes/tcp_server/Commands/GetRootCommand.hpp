#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/Entity.hpp"
#include "includes/Scene.hpp"
#include <iostream>
#include <string>
#define GET_ROOT_COMMAND "GetRoot"
struct GetRootCommand : public TcpCommand
{
	GetRootCommand() = default; 
	std::string execute(Engine * e) override;
	int undo() override;

private:

};
