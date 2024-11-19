#pragma once
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>

struct TcpCommand {
  std::vector<std::string> parameters;
  TcpCommand(int64_t uuid) : _uuid(uuid) {}
  virtual ~TcpCommand() = default;
  virtual int execute() = 0;
  virtual int undo() = 0;
    int64_t _uuid;
  private:
    std::map<std::string, std::string> _previous_state;
};
