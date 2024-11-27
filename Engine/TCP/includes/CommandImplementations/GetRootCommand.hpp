#pragma once
#include "includes/TcpCommand.hpp"
#include "includes/Entity.hpp"
#include "includes/Scene.hpp"
#include <iostream>
#define GET_ROOT_COMMAND "GetRoot"
struct GetRootCommand : public TcpCommand
{
	GetRootCommand(); 
	int execute() override;
	int undo() override;

private:

};

