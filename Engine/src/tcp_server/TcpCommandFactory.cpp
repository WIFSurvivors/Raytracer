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
#include "includes/tcp_server/Commands/ExportJsonCommand.hpp"
#include "includes/tcp_server/Commands/CreateEntityCommand.hpp"
#include "includes/tcp_server/Commands/GetComponentOptionsCommand.hpp"
#include "includes/tcp_server/Commands/SetComponentOptionsCommand.hpp"
#include "includes/tcp_server/Commands/CreateComponentCommand.hpp"
#include "includes/tcp_server/Commands/GetBouncesCommand.hpp"
#include "includes/tcp_server/Commands/SetBouncesCommand.hpp"
#include "includes/tcp_server/Commands/GetLogPath.hpp"
#include "includes/tcp_server/Commands/GetFovCommand.hpp"
#include "includes/tcp_server/Commands/SetFovCommand.hpp"
#include "includes/tcp_server/Commands/UndoCommand.hpp"
#include "includes/tcp_server/Commands/RemoveComponent.hpp"
#include "includes/tcp_server/Commands/SetFrameRate.hpp"
#include "includes/tcp_server/Commands/GetFrameRate.hpp"
#include "includes/tcp_server/Commands/RemoveEntity.hpp"
#include "includes/tcp_server/Commands/PrintTable.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <map>
#include <string>
#include <format>
using  RT::Log;

TcpCommandFactory::TcpCommandFactory() {}

std::unique_ptr<TcpCommand>
TcpCommandFactory::create_command(ParsedTcpCommand parsed_command) {
  uuid _uuid;
  std::string uuid_string = parsed_command.get_uuid();
  std::string command_string = parsed_command.get_command();
  std::vector<std::string> parameters = parsed_command.get_parameters();
  LOG_TCP(
      std::format("Creating command from message: {0} {1} with {2} parameters",
                  command_string, uuid_string, parameters.size()));
  if (!uuid_string.empty() && uuid_string.compare("null") != 0) {
    boost::uuids::string_generator generator;
    _uuid = generator(uuid_string);
  }
  if (command_string.compare(MOVE_COMMAND) == 0) {
    LOG_TCP("Create MoveCommand");
    float x = std::stof(parameters[0]);
    float y = std::stof(parameters[1]);
    float z = std::stof(parameters[2]);
    return std::make_unique<MoveCommand>(_uuid, x, y, z);
  } else if (command_string.compare(ROTATE_COMMAND) == 0) {
    LOG_TCP("Create RotateCommand");
    float x = std::stof(parameters[0]);
    float y = std::stof(parameters[1]);
    float z = std::stof(parameters[2]);
    return std::make_unique<RotateCommand>(_uuid, x, y, z);
  } else if (command_string.compare(SCALE_COMMAND) == 0) {
    LOG_TCP("Create ScaleCommand");
    float x = std::stof(parameters[0]);
    float y = std::stof(parameters[1]);
    float z = std::stof(parameters[2]);
    return std::make_unique<ScaleCommand>(_uuid, x, y, z);
  } else if (command_string.compare(GET_ROOT_COMMAND) == 0) {
    LOG_TCP("Create GetRootCommand");
    return std::make_unique<GetRootCommand>();
  } else if (command_string.compare(CREATE_ENTITY_COMMAND) == 0) {
    LOG_TCP("Create CreateEntityCommand");
    return std::make_unique<CreateEntityCommand>(_uuid, parameters[0]);
  } else if (command_string.compare(CLOSE_RENDER_COMMAND) == 0) {
    LOG_TCP("Create CloseRenderCommand");
    return std::make_unique<CloseRenderCommand>();
  } else if (command_string.compare(GET_ENTITIES_COMMAND) == 0) {
    LOG_TCP("Create GetChildEntitiesCommand");
    return std::make_unique<GetChildEntitiesCommand>(_uuid);
  } else if (command_string.compare(IMPORT_JSON_COMMAND) == 0) {
    LOG_TCP("Create ImportJsonCommand");
    return std::make_unique<ImportJsonCommand>(parameters[0]);
  } else if (command_string.compare(GET_COMPONENTS_COMMAND) == 0) {
    LOG_TCP("Create GetComponentsCommand");
    return std::make_unique<GetComponentsCommand>(_uuid);
  } else if (command_string.compare(GET_ENTITIY_OPTIONS_COMMAND) == 0) {
    LOG_TCP("Create GetEntityOptionsCommand");
    return std::make_unique<GetEntityOptionsCommand>(_uuid);
  } else if (command_string.compare(SET_COMPONENT_OPTIONS_COMMAND) == 0) {
    LOG_TCP("Create SetComponentOptionsCommand");
    return std::make_unique<SetComponentOptions>(
        _uuid, boost::json::parse(parameters[0]).as_object());
  } else if (command_string.compare(GET_COMPONENT_OPTIONS_COMMAND) == 0) {
    LOG_TCP("Create GetComponentOptionsCommand");
    return std::make_unique<GetComponentOptions>(_uuid);
  } else if (command_string.compare(CREATE_COMPONENT_COMMAND) == 0) {
    LOG_TCP("Create CreateComponentCommand");
    return std::make_unique<CreateComponentCommand>(_uuid, parameters[0]);
  } else if (command_string.compare(GET_BOUNCES_COMMAND) == 0) {
    LOG_TCP("Create GetBouncesCommand");
    return std::make_unique<GetBouncesCommand>();
  } else if (command_string.compare(SET_BOUNCES_COMMAND) == 0) {
    LOG_TCP("Create SetBouncesCommand");
    return std::make_unique<SetBouncesCommand>(std::stoi(parameters[0]));
  } else if (command_string.compare(EXPORT_JSON_COMMAND) == 0) {
    LOG_TCP("Create ExportJsonCommand");
    return std::make_unique<ExportJsonCommand>(parameters[0]);
  } else if (command_string.compare(GET_LOG_PATH_COMMAND) == 0) {
    LOG_TCP("Create GetLogPathCommand");
    return std::make_unique<GetLogPath>();
  }
  else if (command_string.compare(GET_FOV_COMMAND) == 0) {
    LOG_TCP("Create GetFovCommand");
    return std::make_unique<GetFovCommand>();
  }
  else if (command_string.compare(SET_FOV_COMMAND) == 0) {
    LOG_TCP("Create SetFovCommand");
    return std::make_unique<SetFovCommand>(std::stof(parameters[0]));
  }
  else if (command_string.compare(UNDO_COMMAND) == 0) {
    LOG_TCP("Create UndoCommand");
    if(parameters.size() > 0) {
      return std::make_unique<UndoCommand>(std::stoi(parameters[0]));
    }
    return std::make_unique<UndoCommand>();
  }
  else if (command_string.compare(REMOVE_COMPONENT_COMMAND) == 0) {
    LOG_TCP("Create RemoveComponentCommand");
    return std::make_unique<RemoveComponent>(_uuid);
  }
  else if (command_string.compare(SET_FRAME_RATE_COMMAND) == 0) {
    LOG_TCP("Create SetFrameRateCommand");
    return std::make_unique<SetFrameRate>(std::stoi(parameters[0]));
  }
  else if (command_string.compare(GET_FRAME_RATE_COMMAND) == 0) {
    LOG_TCP("Create GetFrameRateCommand");
    return std::make_unique<GetFrameRate>();
  }
  else if (command_string.compare(REMOVE_ENTITY_COMMAND) == 0) {
    LOG_TCP("Create RemoveEntityCommand");
    return std::make_unique<RemoveEntity>(_uuid);
  }
  else if (command_string.compare(PRINT_TABLE_COMMAND) == 0) {
    LOG_TCP("Create PrintTableCommand");
    return std::make_unique<PrintTable>();
  }
  else {
    LOG_ERROR("Command not found");
    return nullptr;
  }
}
