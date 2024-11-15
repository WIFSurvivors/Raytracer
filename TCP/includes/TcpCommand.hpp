#pragma once
#include <map>
#include <string>

struct TcpCommand {

  virtual ~TcpCommand() = default;
  virtual int execute() = 0;
  virtual int undo() = 0;
  private:
    std::map<std::string, std::string> _previous_state;
};
