#include "includes/CommandImplementations/GetRootCommand.hpp"
#include "includes/Entity.hpp"
#include "includes/Engine.hpp"
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

std::string GetRootCommand::execute(Engine * engine) 
{
    std::cout << "GetRootCommand::execute()" << std::endl;
    
    if (auto root_entity = engine->get_scene()->get_root().lock())
    {
        boost::json::object root_json = root_entity->to_json();
        std::cout << boost::json::serialize(root_json) << std::endl;
        return boost::json::serialize(root_json);
    } 
    else {
        std::cout << "Root entity is null" << std::endl;
    }   
    
    return "";
}
