#include "includes/tcp_server/TcpCommandFactory.hpp"

TcpCommandFactory::TcpCommandFactory() {}

std::unique_ptr<TcpCommand>
TcpCommandFactory::create_command(ParsedTcpCommand parsed_command) {
  std::cout << "Creating command from message: " << parsed_command.command
            << std::endl;
  uuid _uuid;
  if (!parsed_command.uuid.empty()) {
    _uuid = boost::uuids::string_generator{}(parsed_command.uuid);
  }
  if (parsed_command.command.compare(MOVE_COMMAND) == 0) {
    std::cout << "Create MoveCommand" << std::endl;
    int x = std::stoi(parsed_command.parameters[0]);
    int y = std::stoi(parsed_command.parameters[1]);
    int z = std::stoi(parsed_command.parameters[2]);
    return std::make_unique<MoveCommand>(_uuid, x, y, z);
  } else if (parsed_command.command.compare(ROTATE_COMMAND) == 0) {
    std::cout << "Create RotateCommand from message: " << std::endl;
    int x = std::stoi(parsed_command.parameters[0]);
    int y = std::stoi(parsed_command.parameters[1]);
    int z = std::stoi(parsed_command.parameters[2]);
    return std::make_unique<RotateCommand>(_uuid, x, y, z);
  } else if (parsed_command.command.compare(GET_ROOT_COMMAND) == 0) {
    std::cout << "Create GetRootCommand from message: " << std::endl;
    return std::make_unique<GetRootCommand>();
  } else if (parsed_command.command.compare(CLOSE_RENDER_COMMAND) == 0) {
    std::cout << "Create CloseRenderCommand from message: " << std::endl;
    return std::make_unique<CloseRenderCommand>();
  } else if (parsed_command.command.compare(GET_ENTITIES_COMMAND) == 0) {
    std::cout << "Create GetEntitiesCommand from message: " << std::endl;

    return std::make_unique<GetChildEntitiesCommand>(_uuid);
  } else if (parsed_command.command.compare(GET_COMPONENTS_COMMAND) == 0) {
    std::cout << "Create GetComponentsCommand from message: " << std::endl;

    return std::make_unique<GetComponentsCommand>(_uuid);
  } else if (parsed_command.command.compare(GET_ENTITIY_OPTIONS_COMMAND) == 0) {
    std::cout << "Create GetEntityOptionsCommand from message: " << std::endl;

    return std::make_unique<GetEntityOptionsCommand>(_uuid);
  } else {
    std::cout << "Unknown command" << std::endl;
  }
  return nullptr;
}
