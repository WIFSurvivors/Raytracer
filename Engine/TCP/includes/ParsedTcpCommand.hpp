#pragma once
#include <string>
#include <vector>

struct ParsedTcpCommand {
  std::string command;
  std::string uuid;
  std::vector<std::string> parameters;
  ParsedTcpCommand(std::string command, std::string uuid,
                   std::vector<std::string> parameters);

private:
};
