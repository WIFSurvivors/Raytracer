#include "includes/CommandImplementations/CloseRenderCommand.hpp"
#include "includes/Engine.hpp"

std::string CloseRenderCommand::execute(Engine * e)
{
    e->get_render_system()->destroy();
    return "Render System destroyed";
}
CloseRenderCommand::CloseRenderCommand()
{

}
int CloseRenderCommand::undo()
{
    return 0;
}



