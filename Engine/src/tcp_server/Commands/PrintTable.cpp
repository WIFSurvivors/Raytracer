#include "includes/tcp_server/Commands/PrintTable.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

using RT::Engine;
using RT::Log;

std::string PrintTable::execute(Engine *engine) { 
    auto scene = engine->get_scene();
    if (!scene) {
        LOG_ERROR("Scene is null");
        return "Scene is null";
    }
    scene->print_system_data();
    set_successfull(true);
    return "------ Printed Table ------"; 
    }
