#include "includes/system/System.hpp"
#include "includes/utility/NotSupportedError.hpp"

Component *System::create_component(uuid uuid, Entity *e) {
  throw NotSupportedError{};
}

std::shared_ptr<Entity> System::create_entity(const std::string &name,
                                              uuid uuid) {
  return create_entity("Unnamed", uuid);
}
bool System::remove(Component *c) { throw NotSupportedError{}; }
bool System::remove(Entity *e) { throw NotSupportedError{}; }
