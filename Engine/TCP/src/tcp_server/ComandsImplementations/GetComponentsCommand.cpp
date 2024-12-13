#include "includes/CommandImplementations/GetComponentsCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/JsonConverter.hpp"
#include "includes/Entity.hpp"


std::string GetComponentsCommand::execute(Engine *e) {
    Entity* entity = nullptr;
    if(auto entity = *(*(e->get_scene()))[_uuid])    
    {
        auto components = entity->get_components();
        //return boost::json::serialize(entity_to_json());
    }

    if(entity)
    {
        std::cout << "Entity found" << std::endl;
        return "GetComponentsCommand executed for entity: " + entity->get_name();
    }   
    
  return "Entity not found";
}
int GetComponentsCommand::undo() { return 0; }

