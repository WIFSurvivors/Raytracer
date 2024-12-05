#include "includes/CommandImplementations/GetEntityOptionsCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/JsonConverter.hpp"

std::string GetEntityOptionsCommand::execute(Engine *e) {
    Entity* entity = nullptr;
    if(auto e_sys = dynamic_cast<EntitySystem*>(e->get_active_uuid_manager()->get_system(_uuid)))
    {
        auto entity = std::make_shared<Entity>(*e_sys->get_entities().at(_uuid));
        return boost::json::serialize(entity_options_to_json(entity));
    }

    if(entity)
    {
        std::cout << "Entity found" << std::endl;
        return "GetEntityOptionsCommand executed for entity: " + entity->get_name();
    }   
    
  return "Entity not found";
}
int GetEntityOptionsCommand::undo() { return 0; }