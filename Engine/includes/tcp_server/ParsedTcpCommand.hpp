#pragma once
#include <string>
#include <vector>
/// @brief Parsed command from a tcp message.
/// The parsed command contains the command string, the uuid and the parameters.
struct ParsedTcpCommand {
  /// @brief Constructor for the parsed command.
  /// @param command The command string.
  /// @param uuid The uuid of the command.
  /// @param parameters The parameters of the command.
  ParsedTcpCommand(std::string command, std::string uuid,
                   std::vector<std::string> parameters)
      : command(command), uuid(uuid), parameters(parameters) {}
  inline std::string get_command() const { return command; }
  inline std::string get_uuid() const { return uuid; }
  inline std::vector<std::string> get_parameters() const { return parameters; }
  inline void set_command(std::string command) { this->command = command; }
  inline void set_uuid(std::string uuid) { this->uuid = uuid; }
  inline void set_parameters(std::vector<std::string> parameters) {
    this->parameters = parameters;
  }

private:
  std::string command;
  std::string uuid;
  std::vector<std::string> parameters;
};
