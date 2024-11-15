#include "includes/EntitySystem.hpp"
#include "includes/Entity.hpp"

EntitySystem::EntitySystem() {}

std::shared_ptr<Entity>
EntitySystem::create_entity(std::string name, std::shared_ptr<Entity> parent) {
  return std::shared_ptr<Entity>();
}

std::shared_ptr<Entity>
EntitySystem::create_entity(std::string name, int64_t uuid,
                            std::shared_ptr<Entity> parent) {
  return Entity::create(name, uuid, parent);
}