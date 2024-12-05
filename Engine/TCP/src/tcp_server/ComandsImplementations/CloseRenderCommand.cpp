#include "includes/CommandImplementations/CloseRenderCommand.hpp"
#include "includes/Engine.hpp"


std::string CloseRenderCommand::execute(Engine *e) {
  e->get_window_manager()->close();
  //   e->get_render_system()->destroy(); // decrepted
  return "Render System destroyed";
}
int CloseRenderCommand::undo() { return 0; }
