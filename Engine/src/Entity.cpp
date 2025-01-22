#include "includes/Entity.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <optional>
#include <algorithm>
#include <utility>

namespace RT {
Entity::Entity(const std::string &name, uuid id) : _name{name}, _uuid{id} {}
Entity::Entity(const std::string &name, uuid id, std::shared_ptr<Entity> parent)
    : _name{name}, _uuid{id}, _parent{parent} {}

std::shared_ptr<Entity> Entity::create(const std::string &name, uuid id) {
  return std::make_shared<make_shared_enabler>(name, id);
}

std::shared_ptr<Entity> Entity::create(const std::string &name, uuid id,
                                       std::shared_ptr<Entity> parent) {
  auto e = std::make_shared<make_shared_enabler>(name, id, parent);
  parent->add_child_entity(e->get_ptr());
  return e;
}

bool Entity::add_component(IComponent *c) {
  if (c == nullptr)
    return false;

  auto id = c->get_uuid();
  if (contains_component(id)) {
    return false;
  }
  _components.push_back(id);
  return true;
}

/*std::optional<uuid> Entity::get_component(uuid id) const {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](uuid c_id) { return c_id == id; });

  if (it == _components.end())
    return std::nullopt; // same as "return {};"

  return std::make_optional<uuid>(*it);
}

std::optional<uuid> Entity::get_component(uuid id) {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](uuid c_id) { return c_id == id; });

  if (it == _components.end()) {
    return {};
  }
  return std::make_optional<IComponent *>(*it);
}*/

bool Entity::contains_component(IComponent *c) {
  if (c == nullptr)
    return false;

  contains_component(c->get_uuid());
  return true;
}

bool Entity::contains_component(uuid id) {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](uuid c_id) { return c_id == id; });
  if (it == _components.end()) {
    return false;
  }
  return true;
}

bool Entity::remove_component(IComponent *c) {
  if (c == nullptr)
    return false;

  remove_component(c->get_uuid());
  return true;
}

bool Entity::remove_component(uuid id) {
  auto it = std::find_if(_components.begin(), _components.end(),
                         [id](uuid c_id) { return c_id == id; });
  if (it != _components.end()) {
    _components.erase(it);
    return true;
  }
  return false;
}

bool Entity::remove_components(UUIDManager *um) {
  bool success = true;
  LOG_ERROR(std::format("size: {}", _components.size()));
  for (int i = _components.size() - 1; i >= 0; --i) {
    LOG_ERROR(std::format("i: {}", i));
    // LOG_ERROR(std::format("{}", boost::uuids::to_string(_components[i])));
    if (!um->remove(_components[i])) {
      //   LOG_ERROR(std::format("{}", "awaw"));
      success = false;
      continue;
    }
    _components.erase(_components.begin() + i);
  }
  //   for (auto it = _components.begin(); it != _components.end();) {
  //     LOG_ERROR(std::format("{}", boost::uuids::to_string(*it)));
  //     if (!um->remove(*it)) {
  //       // LOG_ERROR(std::format("{}", "awaw"));
  //       success = false;
  //       ++it;
  //     } else {
  //       it = _components.erase(it);
  //     }
  //   }
  return success;
}

// 1) turn pointer list into index+uuid+is_valid list
//   std::vector<std::tuple<int, uuid, bool>> to_remove;
//   std::transform(_components.begin(), _components.end(),
//                  std::back_inserter(to_remove),
//                  [i = 0](auto &c) mutable -> std::tuple<int, uuid, bool> {
//                    LOG(std::format("mapped: ({}, {})", i,
//                                    boost::uuids::to_string(c->get_uuid())));
//                    return {i++, c->get_uuid(), true};
//                  });
//   LOG("WA");
//   // 2) check if uuid is allowed to be removed
//   std::vector<bool> valid_deletions;
//   for (auto it = to_remove.begin(); it != to_remove.end();) {
//     if (!um->remove(std::get<1>(*it))) {
//       LOG_ERROR(std::format(
//           "invalid component ({}) deletion on entity \"{}\"({})",
//           boost::uuids::to_string(std::get<1>(*it)), this->get_name(),
//           boost::uuids::to_string(this->get_uuid())));
//       std::get<2>(*it) = false;
//       //   valid_deletions[it->first] = false;
//       ++it;
//       continue;
//     }
//     // valid_deletions[it->first] = true;
//     it = to_remove.erase(it);
//     // ++it;
//   }
//   LOG("WB");
//   std::for_each(to_remove.begin(), to_remove.end(), [](auto &thing) {
//     LOG(std::format("VALID?? {} {} {}", std::get<0>(thing),
//                     boost::uuids::to_string(std::get<1>(thing)),
//                     std::get<2>(thing)));
//   });
//   LOG("WC");
//   for (size_t i = to_remove.size(); i >= 0; --i) {
//     if (std::get<2>(to_remove[i]) == true) {
//       _components.erase(_components.begin() + i);
//     }
//   }
//   LOG("WD");
//   // if any value is
//   return !std::any_of(valid_deletions.begin(), valid_deletions.end(),
//                       [](bool t) { return t == false; });
// return true;
// }

void Entity::add_child_entity(std::shared_ptr<Entity> e) {
  // TODO: check that 'e' doesn't have a parent....
  _child_entities.push_back(e);
}

bool Entity::remove_child_entity(std::shared_ptr<Entity> e) {
  return remove_child_entity(e.get());
}

bool Entity::remove_child_entity(Entity *e) {
  auto it = std::find_if(_child_entities.begin(), _child_entities.end(),
                         [e](auto e_ptr) { return e_ptr.get() == e; });

  if (it == _child_entities.end()) {
    return false;
  }
  _child_entities.erase(it);
  return true;
}

glm::vec3 Entity::get_world_position() const {
  if (auto p = _parent.lock()) { // recursion ends if there is no parent (aka
                                 // it's the root)
    auto vec = p->get_world_position();
    return vec + get_local_position();
  }
  return get_local_position();
}

glm::vec3 Entity::get_world_rotation() const {
  if (auto p = _parent.lock()) { // recursion ends if there is no parent (aka
                                 // it's the root)
    auto vec = p->get_world_rotation();
    return vec * get_local_rotation();
  }
  return get_local_rotation();
}

glm::vec3 Entity::get_world_scale() const {
  if (auto p = _parent.lock()) { // recursion ends if there is no parent (aka
                                 // it's the root)
    auto vec = p->get_world_scale();
    return vec * get_local_scale();
  }
  return get_local_scale();
}

boost::json::object Entity::to_json() const {
  boost::json::object obj = self_to_json();

  boost::json::array children;
  children = std::move(children_to_json());
  obj["children"] = std::move(children);
  return obj;
}

boost::json::object Entity::self_to_json() const {
  boost::json::object obj;
  obj["uuid"] = boost::uuids::to_string(get_uuid());
  obj["name"] = get_name();
  obj["children_count"] = get_child_entities().size();
  return obj;
}

boost::json::array Entity::children_to_json() const {
  boost::json::array arr;
  for (const auto &e : get_child_entities()) {
    boost::json::object obj = e->self_to_json();
    arr.push_back(std::move(obj));
  }
  return arr;
}

void Entity::print() { LOG(boost::json::serialize(to_json())); }
} // namespace RT
