#include "includes/tcp_server/Commands/GetChildEntitiesCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/JsonConverter.hpp"



std::string GetChildEntitiesCommand::execute(Engine *e) {
    std::cout << boost::uuids::to_string(_uuid) << std::endl;
    auto scene = e->get_scene();
    std::cout << "GetChildEntitiesCommand::execute()1" << std::endl;
    if (!scene) {
        std::cerr << "Scene pointer is null" << std::endl;
        return "";
    }
    std::cout << "GetChildEntitiesCommand::execute()2" << std::endl;
    auto entity = scene->get_entity(_uuid);
    std::cout << "GetChildEntitiesCommand::execute()3" << std::endl;
    if (!entity.has_value()) {
        std::cerr << "Entity pointer is null" << std::endl;
        return "";
    }
    std::cout << "GetChildEntitiesCommand::execute()4" << std::endl;
    auto json_entity = entity.value()->to_json();
    std::cout << "GetChildEntitiesCommand::execute()5" << std::endl;
    std::cout << boost::json::serialize(json_entity) << std::endl;
    return boost::json::serialize(json_entity);
  
  return "End of GetChildEntitiesCommand";
}
int GetChildEntitiesCommand::undo() { return 0; }