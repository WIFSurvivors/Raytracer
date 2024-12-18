#include "includes/tcp_server/Commands/GetEntityOptionsCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/JsonConverter.hpp"

std::string GetEntityOptionsCommand::execute(Engine *e) {
     try {
        auto scene = e->get_scene();
        if (!scene) {
            std::cerr << "Scene is null" << std::endl;
            return "Scene is null";
        }

        auto entity_ptr = (*scene)[_uuid];
        if (!entity_ptr) {
            std::cerr << "Entity not found for UUID: " << boost::uuids::to_string(_uuid) << std::endl;
            return "Entity not found";
        }

        auto entity = *entity_ptr;
        if (!entity) {
            std::cerr << "Entity is null for UUID: " << boost::uuids::to_string(_uuid) << std::endl;
            return "Entity is null";
        }

        auto entity_json = entity_options_to_json(std::make_shared<Entity>(*entity));
        return boost::json::serialize(entity_json);
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
        return "Memory allocation failed";
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return "Exception occurred";
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return "Unknown exception occurred";
    }
}
int GetEntityOptionsCommand::undo() { return 0; }
