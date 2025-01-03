#pragma once

#include "includes/UUIDManager.hpp"
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <map>
#include <string>

struct Entity;

/**
 * EntityStorage creates new Entities without Ownership. Ownership is on the
 * Scene that is creating that Entity. Most importantly, it maps UUIDs to
 * Entities Pointers.
 */
struct EntityStorage : public IStorage {
  using uuid = boost::uuids::uuid;
  
  EntityStorage();

  inline virtual const std::string get_name() const final {
    return "Entity Storage";
  }

  /**
   * A root entity doesn't have a parent and is used for generating new scenes.
   */
  std::shared_ptr<Entity> create_root_entity(const std::string &name, uuid id);

  /**
   * Create a entites with a given uuid, name and parent entity.
   */
  std::shared_ptr<Entity> create_entity(const std::string &name, uuid id,
                                        std::shared_ptr<Entity> parent);

  /**
   * Get Entity stored in this system. Will return std::nullopt when UUID is
   * not found.
   */
  inline std::optional<Entity *> get_entity(uuid id) {
    return _entities.contains(id) ? std::make_optional(_entities[id])
                                  : std::nullopt;
  }

  /**
   * Removes an entity from the underlying data structure by uuid.
   * THIS DOES NOT REMOVE IT FROM THE ACTIVE SCENE!!!
   * THIS DOES NOT CORRENTLY REMOVE ALL LINKED COMPONENTS!!!
   */
  bool remove(uuid id);

  /**
   * Removes an entity from the underlying data structure by uuid.
   * THIS DOES NOT REMOVE IT FROM THE ACTIVE SCENE!!!
   * THIS DOES NOT CORRENTLY REMOVE ALL LINKED COMPONENTS!!!
   */
  bool remove(Entity *e);

  /**
   * Display all currently saved entites by showing their uuid, translation
   * (position, rotation, scale), component and children entity count and their
   * parent entity uuid.
   */
  void print();

private:
  std::map<uuid, Entity *> _entities{};
};
