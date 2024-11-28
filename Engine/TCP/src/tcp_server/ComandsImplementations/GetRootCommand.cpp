#include "includes/CommandImplementations/GetRootCommand.hpp"
#include "includes/Entity.hpp"
#include "includes/Scene.hpp"
#include <boost/json.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
GetRootCommand::GetRootCommand() 
{
    std::cout << "GetRootCommand::GetRootCommand()" << std::endl;
   
}

int GetRootCommand::undo() 
{
    std::cout << "GetRootCommand::undo()" << std::endl;
    return 0;
}

boost::json::array entities_to_json(const std::vector<std::shared_ptr<Entity>>& entities) {
    boost::json::array arr;
    for (const auto& entity : entities) {
        boost::json::object obj;
        obj["uuid"] = boost::uuids::to_string(entity->get_uuid());
        obj["name"] = entity->get_name();
        obj["children_count"] = entity->get_child_entities().size();
        arr.push_back(std::move(obj));
    }
    return arr;
}

boost::json::object entity_to_json(const std::shared_ptr<Entity>& entity) {
    boost::json::object obj;
    obj["uuid"] = boost::uuids::to_string(entity->get_uuid());
    obj["name"] = entity->get_name();
    obj["children_count"] = entity->get_child_entities().size();

    boost::json::array children;
    children = std::move(entities_to_json(entity->get_child_entities()));
    obj["children"] = std::move(children);

    return obj;
}


std::string GetRootCommand::execute(Engine * engine) 
{
    std::cout << "GetRootCommand::execute()" << std::endl;
    
    if (auto root_entity = engine->get_scene()->get_root().lock())
    {
        boost::json::object root_json = entity_to_json(root_entity);
        std::cout << boost::json::serialize(root_json) << std::endl;
        return boost::json::serialize(root_json);
    } 
    else {
        std::cout << "Root entity is null" << std::endl;
    }
    
    
    return "";
}
