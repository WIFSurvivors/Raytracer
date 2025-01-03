#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <boost/uuid/uuid.hpp>
#include <string>

#define CREATE_COMPONENT_COMMAND "CreateComponent"

struct CreateComponentCommand : public TcpCommand
{
	explicit CreateComponentCommand(uuid uuid, std::string type) : TcpCommand(uuid), _type(type) {}
	std::string execute(Engine *e) override;
	int undo() override;

	uuid _uuid_component;
	std::string _type; 
private:
};
