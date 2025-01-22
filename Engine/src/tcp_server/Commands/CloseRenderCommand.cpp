#include "includes/tcp_server/Commands/CloseRenderCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/NotImplementedError.hpp"

using RT::Engine;

std::string CloseRenderCommand::execute(Engine *e) {
  e->get_window_manager()->close();
  //   e->get_render_system()->destroy(); // decrepted
  return "Render System destroyed";
}
