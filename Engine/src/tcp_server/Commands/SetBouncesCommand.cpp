#include "includes/tcp_server/Commands/SetBouncesCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

std::string SetBouncesCommand::execute(Engine *engine)
{
    if(!engine) {
        LOG_ERROR("Engine is null");
        return "Engine is null";
    }
    auto *scene = engine->get_scene();
    if (!scene) {
        LOG_ERROR("Scene is null");
        return "Scene is null";
    }
    auto render_system = scene->get_render_system();
    if (!render_system) {
        LOG_ERROR("RenderSystem is null");
        return "RenderSystem is null";
    }
    render_system->set_bounces(_bounces);

    return "Bounces set to " + std::to_string(_bounces);
}

std::string SetBouncesCommand::undo()
{
    throw NotImplementedError();
}