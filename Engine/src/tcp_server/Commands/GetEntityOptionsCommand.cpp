#include "includes/tcp_server/Commands/GetEntityOptionsCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/JsonConverter.hpp"
#include "includes/Entity.hpp"
#include "includes/Scene.hpp"
#include "includes/utility/Log.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <string>
#include "includes/utility/NotImplementedError.hpp"



std::string GetEntityOptionsCommand::execute(Engine *e) {
     try {
        auto scene = e->get_scene();
        if (!scene) {
            std::string msg = "Scene is null";
            Log::error(msg);
            return msg;
        }

        auto entity_ptr = (*scene)[_uuid];
        if (!entity_ptr) {
            std::string msg = "Entity not found";
            Log::error(msg);
            return msg;
        }

        auto entity = *entity_ptr;
        if (!entity) {
            std::string msg = "Entity is null";
            Log::error(msg);
            return msg;
        }
        auto entity_json = entity_options_to_json(std::make_shared<Entity>(*entity));
        return boost::json::serialize(entity_json);
    } catch (const std::bad_alloc& e) {
        Log::error(std::format("Memory allocation failed: {}", e.what()));
        return "Memory allocation failed";
    } catch (const std::exception& e) {
        Log::error(std::format("Exception: {}", e.what()));
        return "Exception occurred";
    } catch (...) {
        Log::error("Unknown exception occurred");
        return "Unknown exception occurred";
    }
}
int GetEntityOptionsCommand::undo() { throw NotImplementedError{}; }
