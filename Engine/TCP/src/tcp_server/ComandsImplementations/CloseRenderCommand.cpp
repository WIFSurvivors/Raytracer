#include "includes/CommandImplementations/CloseRenderCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/system/RenderSystem.hpp"

CloseRenderCommand::CloseRenderCommand()
{

}
std::string CloseRenderCommand::execute(Engine * e)
{
    e->get_render_system()->destroy();
    return "Render System destroyed";
}
int CloseRenderCommand::undo()
{
    return 0;
}



