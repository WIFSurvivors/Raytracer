#pragma once
#include "includes/TcpCommand.hpp"
#include <string>
#define CLOSE_RENDER_COMMAND "CloseRender"

struct CloseRenderCommand : public TcpCommand {
  CloseRenderCommand() = default;
  std::string execute(Engine *e) override;
  int undo() override;

private:
};
