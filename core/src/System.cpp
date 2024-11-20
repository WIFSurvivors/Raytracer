#include "includes/System.hpp"

// #include "includes/utility/NotImplementedError.hpp"
#include "includes/utility/NotSupportedError.hpp"

Component *System::create_component(int64_t uuid, std::shared_ptr<Entity> e) {
  throw NotSupportedError{};
}

std::shared_ptr<Entity> System::create_entity(int64_t uuid) {
  return create_entity("Unnamed", uuid);
}
std::shared_ptr<Entity> System::create_entity(const std::string &name,
                                              int64_t uuid) {
  throw NotSupportedError{};
}
bool System::remove(Component *c) { throw NotSupportedError{}; }
bool System::remove(Entity *e) { throw NotSupportedError{}; }
