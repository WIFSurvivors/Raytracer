#include "includes/tcp_server/Commands/GetRootCommand.hpp"
#include "includes/Entity.hpp"
#include "includes/Engine.hpp"
#include "includes/Scene.hpp"
#include <boost/json.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include "includes/utility/JsonConverter.hpp"


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
    try
    {
        if (!engine) {
            std::cerr << "Engine pointer is null" << std::endl;
            return "";
        }

        auto scene = engine->get_scene();
        if (!scene) {
            std::cerr << "Scene pointer is null" << std::endl;
            return "";
        }

        auto root_entity = scene->get_root().lock();
        if (root_entity) {
            std::cout << "Root entity is not null" << std::endl;
            std::cout << "Root entity UUID: " << root_entity->get_uuid() << std::endl;
            std::cout << "Root entity Name: " << root_entity->get_name() << std::endl;

            // Check the state of root_entity before calling to_json
            if (root_entity->get_uuid().is_nil()) {
                std::cerr << "Root entity UUID is nil" << std::endl;
            }
            if (root_entity->get_name().empty()) {
                std::cerr << "Root entity name is empty" << std::endl;
            }

            // Add more logging before calling to_json
            std::cout << "Calling root_entity->to_json()" << std::endl;
            boost::json::object root_json = root_entity->to_json();
            std::cout << "Root entity to json" << std::endl;
            std::cout << boost::json::serialize(root_json) << std::endl;
            return boost::json::serialize(root_json);
        } 
        else {
            std::cout << "Root entity is null" << std::endl;
        }   
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return "";
}