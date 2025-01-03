#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <boost/uuid/uuid.hpp>
#include <string>

#define CREATE_ENTITY_COMMAND "CreateEntity"

struct CreateEntityCommand : public TcpCommand
{
	explicit CreateEntityCommand(uuid uuid, std::string entity_name) : TcpCommand(uuid), _entity_name(entity_name) {}
	std::string execute(Engine *e) override;
	int undo() override;

private:
	std::string _entity_name;
};
