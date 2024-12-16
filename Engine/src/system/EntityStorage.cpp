#include "includes/system/EntityStorage.hpp"
#include "includes/Entity.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include "includes/utility/TablePrinter.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
// #include "glm/vec3.hpp"
#include "glm/ext.hpp"

EntityStorage::EntityStorage() {
  SimpleLogger::print("-- created entity storage");
}

std::shared_ptr<Entity> EntityStorage::create_root_entity(const std::string &name,
                                                  uuid id) {
  auto e = Entity::create(name, id);
  _entities[id] = e.get();
  return e;
}

std::shared_ptr<Entity>
EntityStorage::create_entity(const std::string &name, uuid id,
                            std::shared_ptr<Entity> parent) {
  auto e = Entity::create(name, id, parent);
  _entities[id] = e.get();
  return e;
}
bool EntityStorage::remove(uuid id) {
  auto it = std::find_if(_entities.begin(), _entities.end(),
                         [id](const auto &e) { return e.first == id; });
  if (it != _entities.end()) {
    _entities.erase(it);
    return true;
  }
  return false;
}

bool EntityStorage::remove(Entity *e) {
  auto it = std::find_if(_entities.begin(), _entities.end(),
                         [e](const auto &val) { return val.second == e; });
  if (it != _entities.end()) {
    _entities.erase(it);
    return true;
  }
  return false;
}

void EntityStorage::print() {
  std::cout << "Entity Storage: "<< std::endl;
  VariadicTable<std::string, std::string, size_t, size_t, std::string> vt(
      {"Entity UUID", "Entity Name", "CE", "CC", "World Position"});
      std::cout << "Entity UUID" << std::endl;
  for (const auto &[id, e] : _entities) {
    std::cout << "Entity UUID3 " << boost::uuids::to_string(id) << std::endl;
    std::cout << "Entity UUID3.1 " << e->get_name()<< std::endl;
        std::cout << "Entity UUID3.2 " <<  e->get_child_entities().size()<< std::endl;
        std::cout << "Entity UUID3.3 " <<  e->get_components().size() << std::endl;
        std::cout << "Entity UUID3.4 " <<  glm::to_string(e->get_world_position())<< std::endl;
        std::cout << "Entity UUID3.5 " << std::endl;
    vt.addRow(boost::uuids::to_string(id),
			  e->get_name(),
			  e->get_child_entities().size(),
			  e->get_components().size(),
			  glm::to_string(e->get_world_position())
			  /*, e->get_world_position()*/); // <- unfortunately looks weird :C
        std::cout << "Entity UUID4" << std::endl;
  }
  std::cout << "Entity UUID2" << std::endl;
  vt.print(std::cout);
  std::cout << "Legend: \n"
			<< "\t-CE = Count Child Entities\n"
			<< "\t-CC = Count Components\n";
  std::cout << std::endl;
}
