#include "includes/tcp_server/Commands/GetFovCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

std::string GetFovCommand::execute(Engine *engine)
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
    auto camera_system = scene->get_camera_system();
    if (!camera_system) {
        LOG_ERROR("RenderSystem is null");
        return "RenderSystem is null";
    }
    float _fov = camera_system->get_main_camera()->get_fov();

    return std::to_string(_fov);
}

std::string GetFovCommand::undo()
{
    throw NotImplementedError();
}
