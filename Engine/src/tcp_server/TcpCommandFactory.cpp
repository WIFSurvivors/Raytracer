#include "includes/tcp_server/TcpCommandFactory.hpp"
#include "includes/utility/Log.hpp"
#include "includes/tcp_server/Commands/MoveCommand.hpp"
#include "includes/tcp_server/Commands/RotateCommand.hpp"
#include "includes/tcp_server/Commands/ScaleCommand.hpp"
#include "includes/tcp_server/Commands/GetRootCommand.hpp"
#include "includes/tcp_server/Commands/CloseRenderCommand.hpp"
#include "includes/tcp_server/Commands/GetChildEntitiesCommand.hpp"
#include "includes/tcp_server/Commands/GetComponentsCommand.hpp"
#include "includes/tcp_server/Commands/GetEntityOptionsCommand.hpp"
#include "includes/tcp_server/Commands/ImportJsonCommand.hpp"
#include "includes/tcp_server/Commands/CreateEntityCommand.hpp"
#include "includes/tcp_server/Commands/GetComponentOptionsCommand.hpp"
#include "includes/tcp_server/Commands/SetComponentOptionsCommand.hpp"
#include "includes/tcp_server/Commands/CreateComponentCommand.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <map>
#include <string>
#include <iostream>




TcpCommandFactory::TcpCommandFactory() {}

std::unique_ptr<TcpCommand>
TcpCommandFactory::create_command(ParsedTcpCommand parsed_command) {
  LOG_TCP("Creating command from message: " + parsed_command.command +
          " with UUID: " + parsed_command.uuid);
  uuid _uuid;
  if (!parsed_command.uuid.empty() &&
      parsed_command.uuid.compare("null") != 0) {
    boost::uuids::string_generator generator;
    _uuid = generator(parsed_command.uuid);
  }
  if (parsed_command.command.compare(MOVE_COMMAND) == 0) {
    LOG_TCP("Create MoveCommand");
    float x = std::stof(parsed_command.parameters[0]);
    float y = std::stof(parsed_command.parameters[1]);
    float z = std::stof(parsed_command.parameters[2]);
    return std::make_unique<MoveCommand>(_uuid, x, y, z);
  } else if (parsed_command.command.compare(ROTATE_COMMAND) == 0) {
    LOG_TCP("Create RotateCommand");
    float x = std::stof(parsed_command.parameters[0]);
    float y = std::stof(parsed_command.parameters[1]);
    float z = std::stof(parsed_command.parameters[2]);
    return std::make_unique<RotateCommand>(_uuid, x, y, z);
  } else if (parsed_command.command.compare(SCALE_COMMAND) == 0) {
    LOG_TCP("Create ScaleCommand");
    float x = std::stof(parsed_command.parameters[0]);
    float y = std::stof(parsed_command.parameters[1]);
    float z = std::stof(parsed_command.parameters[2]);
    return std::make_unique<ScaleCommand>(_uuid, x, y, z);
  } else if (parsed_command.command.compare(GET_ROOT_COMMAND) == 0) {
    LOG_TCP("Create GetRootCommand");
    return std::make_unique<GetRootCommand>();
  } else if (parsed_command.command.compare(CREATE_ENTITY_COMMAND) == 0) {
    LOG_TCP("Create CreateEntityCommand");
    return std::make_unique<CreateEntityCommand>(_uuid,
                                                 parsed_command.parameters[0]);
  } else if (parsed_command.command.compare(CLOSE_RENDER_COMMAND) == 0) {
    LOG_TCP("Create CloseRenderCommand");
    return std::make_unique<CloseRenderCommand>();
  } else if (parsed_command.command.compare(GET_ENTITIES_COMMAND) == 0) {
    LOG_TCP("Create GetChildEntitiesCommand");
    return std::make_unique<GetChildEntitiesCommand>(_uuid);
  } else if (parsed_command.command.compare(IMPORT_JSON_COMMAND) == 0) {
    LOG_TCP("Create ImportJsonCommand");
    return std::make_unique<importJsonCommand>(parsed_command.parameters[0]);
  } else if (parsed_command.command.compare(GET_COMPONENTS_COMMAND) == 0) {
    LOG_TCP("Create GetComponentsCommand");
    return std::make_unique<GetComponentsCommand>(_uuid);
  } else if (parsed_command.command.compare(GET_ENTITIY_OPTIONS_COMMAND) == 0) {
    LOG_TCP("Create GetEntityOptionsCommand");
    return std::make_unique<GetEntityOptionsCommand>(_uuid);
  } else if (parsed_command.command.compare(SET_COMPONENT_OPTIONS_COMMAND) == 0) {
    LOG_TCP("Create SetComponentOptionsCommand");
    return std::make_unique<SetComponentOptions>(_uuid, boost::json::parse(parsed_command.parameters[0]).as_object());
  } else if (parsed_command.command.compare(GET_COMPONENT_OPTIONS_COMMAND) == 0) {
    LOG_TCP("Create GetComponentOptionsCommand");
    return std::make_unique<GetComponentOptions>(_uuid);
  }
  else if (parsed_command.command.compare(CREATE_COMPONENT_COMMAND) == 0) {
    LOG_TCP("Create CreateComponentCommand");
    return std::make_unique<CreateComponentCommand>(_uuid, parsed_command.parameters[0]);
  }
  else {
    LOG_ERROR("Command not found");
    return nullptr;
  }
}
