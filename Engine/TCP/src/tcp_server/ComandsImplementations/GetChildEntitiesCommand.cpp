#include "includes/CommandImplementations/GetChildEntitiesCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/JsonConverter.hpp"



std::string GetChildEntitiesCommand::execute(Engine *e) {
    Entity* entity = nullptr;
    if(auto e_sys = dynamic_cast<EntitySystem*>(e->get_active_uuid_manager()->get_system(_uuid)))
    {
        auto entity = std::make_shared<Entity>(*e_sys->get_entities().at(_uuid));
        return boost::json::serialize(entity_to_json(entity));
    }

    if(entity)
    {
        std::cout << "Entity found" << std::endl;
        return "GetChildEntitiesCommand executed for entity: " + entity->get_name();
    }   
    
  return "Entity not found";
}
int GetChildEntitiesCommand::undo() { return 0; }