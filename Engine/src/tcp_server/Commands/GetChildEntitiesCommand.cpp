#include "includes/tcp_server/Commands/GetChildEntitiesCommand.hpp"
#include "includes/Engine.hpp"



std::string GetChildEntitiesCommand::execute(Engine *e) {
    std::cout << boost::uuids::to_string(_uuid) << std::endl;
    auto scene = e->get_scene();
    if (!scene) {
        std::cerr << "Scene pointer is null" << std::endl;
        return "Scene not found";
    }
    auto entity = scene->get_entity(_uuid);
    if (!entity.has_value()) {
        std::cerr << "Entity pointer is null" << std::endl;
        return "Entity not found";
    }
    auto json_entity = entity.value()->to_json();
    std::cout << boost::json::serialize(json_entity) << std::endl;
    return boost::json::serialize(json_entity);
  
  return "End of GetChildEntitiesCommand";
}
int GetChildEntitiesCommand::undo() { return 0; }