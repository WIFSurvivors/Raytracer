#include "includes/TcpCommandFactory.hpp"

TcpCommandFactory::TcpCommandFactory() {}

std::unique_ptr<TcpCommand> TcpCommandFactory::create_command(ParsedTcpCommand parsed_command) {
  std::cout << "Creating command from message: " << parsed_command.command << std::endl;
    int64_t uuid = std::stoll(parsed_command.uuid);
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
  } else {
    std::cout << "Unknown command" << std::endl;
  }
  return nullptr;
}
