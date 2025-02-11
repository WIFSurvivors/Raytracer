#include "includes/system/EntityStorage.hpp"
#include "includes/Entity.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/VariadicTable.hpp"
#include <format>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <iomanip> // for std::setprecision

#define GLM_ENABLE_EXPERIMENTAL
// #include "glm/vec3.hpp"
#include "glm/ext.hpp"

namespace RT {
EntityStorage::EntityStorage(std::shared_ptr<UUIDManager> um)
    : Storage<Entity *>(um) {
  LOG(std::format("created {}", get_name()));
}

std::shared_ptr<Entity>
EntityStorage::create_root_entity(const std::string &name) {
  auto id = _um->create(this);
  auto e = Entity::create(name, id);
  LOG(std::format("scene: create_root_entity(name): \"{}\", [uuid:{}]\"",
                  e->get_name(), boost::uuids::to_string(e->get_uuid())));
  _storage[id] = e.get();
  return e;
}

std::shared_ptr<Entity>
EntityStorage::create_root_entity(const std::string &name, uuid id) {
  if (!_um->add(id, this)) {
    return nullptr;
  }
  auto e = Entity::create(name, id);
  LOG(std::format("scene: create_root_entity(name, uuid): \"{}\", {}\"",
                  e->get_name(), boost::uuids::to_string(e->get_uuid())));
  _storage[id] = e.get();
  return e;
}

std::shared_ptr<Entity>
EntityStorage::create_entity(const std::string &name,
                             std::shared_ptr<Entity> parent) {
  auto id = _um->create(this);
  auto e = Entity::create(name, id, parent);
  LOG(std::format(
      "scene: create_entity(name, parent): \"{}\", [uuid:{},] \"{}\"",
      e->get_name(), boost::uuids::to_string(e->get_uuid()),
      parent->get_name()));
  _storage[id] = e.get();
  return e;
}

std::shared_ptr<Entity>
EntityStorage::create_entity(const std::string &name, uuid id,
                             std::shared_ptr<Entity> parent) {
  if (!_um->add(id, this)) {
    return nullptr;
  }
  auto e = Entity::create(name, id, parent);
  LOG(std::format(
      "scene: create_entity(name, uuid, parent): \"{}\", {}, \"{}\"",
      e->get_name(), boost::uuids::to_string(e->get_uuid()),
      parent->get_name()));
  _storage[id] = e.get();
  return e;
}

bool EntityStorage::remove(uuid id) {
  if (!_storage.contains(id)) { return false; }
  _um->remove_without_system(id);
  _storage.erase(id);
  return true;
}

bool EntityStorage::remove(Entity *e) { return remove(e->get_uuid()); }

void EntityStorage::print() {
  std::cout << "Entity Storage: " << std::endl;
  VariadicTable<std::string, std::string, size_t, size_t, float, float, float,
                float, float, float, float, float, float>
      vt({"Entity UUID", "Entity Name", "CE", "CC", "P_x", "P_y", "P_z", "R_x",
          "R_y", "R_z", "S_x", "S_y", "S_z"});

  for (const auto &[id, e] : _storage) {
    vt.addRow(boost::uuids::to_string(id), e->get_name(),
              e->get_child_entities().size(), e->get_components().size(),
              e->get_world_position().x, e->get_world_position().y,
              e->get_world_position().z, e->get_world_rotation().x,
              e->get_world_rotation().y, e->get_world_rotation().z,
              e->get_world_scale().x, e->get_world_scale().y,
              e->get_world_scale().z);
  }

  std::streamsize defaultPrecision = std::cout.precision();
  std::cout << std::setprecision(3);
  vt.print(std::cout);
  std::cout << "Legend: \n"
            << "\t-CE = Count Child Entities\n"
            << "\t-CC = Count Components\n";
  std::cout << std::setprecision(defaultPrecision) << std::endl;
  std::cout << std::endl;
}
} // namespace RT
