#pragma once

struct TcpCommand {

  virtual ~TcpCommand() = default;

  virtual void execute() = 0;
  virtual void undo() = 0;
};
