#pragma once
#include "includes/TcpCommand.hpp"
#include "includes/Entity.hpp"
#include "includes/Scene.hpp"
#include <iostream>
#include <string>
#define GET_ROOT_COMMAND "GetRoot"
struct GetRootCommand : public TcpCommand
{
	GetRootCommand(); 
	const std::string& execute(Engine * e) override;
	const std::string& undo() override;

private:

};

