#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define CREATE_COMPONENT_COMMAND "CreateComponent"

struct CreateComponentCommand : public TcpCommand
{
	explicit CreateComponentCommand(uuid uuid, std::string type) : TcpCommand(uuid), _type(type) {}
	std::string execute(Engine *e) override;
	std::string undo() override;

	inline std::string get_type() const { return _type; }
	inline void set_type(std::string type) { _type = type; }
	inline uuid get_uuid_component() const { return _uuid_component; }
	inline void set_uuid_component(uuid uuid_component) { _uuid_component = uuid_component; }
private:
	uuid _uuid_component;
	std::string _type; 
};
