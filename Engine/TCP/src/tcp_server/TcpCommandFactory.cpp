#include "includes/TcpCommandFactory.hpp"

TcpCommandFactory::TcpCommandFactory() {}

std::unique_ptr<TcpCommand> TcpCommandFactory::create_command(ParsedTcpCommand parsed_command) {
  std::cout << "Creating command from message: " << parsed_command.command << std::endl;
    int64_t uuid = 0;
    if (!parsed_command.uuid.empty())
    {
      uuid = std::stoll(parsed_command.uuid);
    }
  if (parsed_command.command.compare(MOVE_COMMAND) == 0) {
    std::cout << "Create MoveCommand" << std::endl;
    int x = std::stoi(parsed_command.parameters[0]);
    int y = std::stoi(parsed_command.parameters[1]);
    int z = std::stoi(parsed_command.parameters[2]);
    return std::make_unique<MoveCommand>(uuid, x, y, z);
  } else if (parsed_command.command.compare(ROTATE_COMMAND) == 0) {
    std::cout << "Create RotateCommand from message: " << std::endl;
    int x = std::stoi(parsed_command.parameters[0]);
    int y = std::stoi(parsed_command.parameters[1]);
    int z = std::stoi(parsed_command.parameters[2]);
    return std::make_unique<RotateCommand>(uuid, x, y, z);
  }
  else if  (parsed_command.command.compare(GET_ROOT_COMMAND) == 0) {
    std::cout << "Create GetRootCommand from message: " << std::endl;
    return std::make_unique<GetRootCommand>();
  }
  else if (parsed_command.command.compare(CLOSE_RENDER_COMMAND) == 0) {
    std::cout << "Create CloseRenderCommand from message: " << std::endl;
    return std::make_unique<CloseRenderCommand>();
  }
  else {
    std::cout << "Unknown command" << std::endl;
  }
  return nullptr;
}
