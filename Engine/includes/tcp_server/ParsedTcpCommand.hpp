#pragma once
#include <string>
#include <vector>

struct ParsedTcpCommand {
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
