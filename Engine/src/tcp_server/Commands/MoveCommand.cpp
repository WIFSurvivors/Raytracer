#include "includes/tcp_server/Commands/MoveCommand.hpp"
#include <boost/uuid/uuid_io.hpp>
#include "includes/Engine.hpp"
#include "includes/Entity.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"
std::string MoveCommand::execute(Engine * engine) {
    auto scene = engine->get_scene();
        if (!scene) {
            std::cerr << "Scene is null" << std::endl;
            return "Scene is null";
        }

        auto entity_ptr = (*scene)[_uuid];
        if (!entity_ptr) {
            std::cerr << "Entity not found for UUID: " << boost::uuids::to_string(_uuid) << std::endl;
            return "Entity not found";
        }

        auto entity = *entity_ptr;
        if (!entity) {
            std::cerr << "Entity is null for UUID: " << boost::uuids::to_string(_uuid) << std::endl;
            return "Entity is null";
        }
        entity->set_local_position(_new_position);
        

    std::cout << "MoveCommand executed on" << boost::uuids::to_string(_uuid) << "with new x y z: " << entity->get_local_position() <<std::endl; 
    return "MoveCommand executed";
}
int MoveCommand::undo() { return 0; }
