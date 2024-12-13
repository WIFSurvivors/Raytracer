#include "includes/CommandImplementations/GetChildEntitiesCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/JsonConverter.hpp"



std::string GetChildEntitiesCommand::execute(Engine *e) {
    Entity* entity = nullptr;
    std::cout << boost::uuids::to_string(_uuid) << std::endl;
    if(auto entity = *(*(e->get_scene()))[_uuid])
    {
       if (!entity)
        {
            return "Entity not found";
        }
        return boost::json::serialize(entity_to_json(std::make_shared<Entity>(*entity)));
    }
    
  return "End of GetChildEntitiesCommand";
}
int GetChildEntitiesCommand::undo() { return 0; }