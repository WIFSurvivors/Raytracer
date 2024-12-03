#pragma once
#include "includes/TcpCommand.hpp"

#define CLOSE_RENDER_COMMAND "CloseRender"

struct CloseRenderCommand : public TcpCommand {
  CloseRenderCommand();
  std::string execute(Engine *e) override;
  int undo() override;

private:
};
