#pragma once 
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/json.hpp>
#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/RenderComponent.hpp"


inline boost::json::array entities_to_json(const std::vector<std::shared_ptr<Entity>>& entities) {
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

inline boost::json::object entity_to_json(const std::shared_ptr<Entity>& entity) {
    boost::json::object obj;
    obj["uuid"] = boost::uuids::to_string(entity->get_uuid());
    obj["name"] = entity->get_name();
    obj["children_count"] = entity->get_child_entities().size();

    boost::json::array children;
    children = std::move(entities_to_json(entity->get_child_entities()));
    obj["children"] = std::move(children);

    return obj;
}