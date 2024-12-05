#include "includes/CommandImplementations/GetComponentsCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/JsonConverter.hpp"
#include "includes/Entity.hpp"


std::string GetComponentsCommand::execute(Engine *e) {
    Entity* entity = nullptr;
    if(auto e_sys = dynamic_cast<EntitySystem*>(e->get_active_uuid_manager()->get_system(_uuid)))
    {
        auto entity = std::make_shared<Entity>(*e_sys->get_entities().at(_uuid));
        //return boost::json::serialize(entity_to_json());
    }

    if(entity)
    {
        std::cout << "Entity found" << std::endl;
        return "GetComponentsCommand executed for entity: " + entity->get_name();
    }   
    
  return "Entity not found";
}

