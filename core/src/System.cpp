#include "includes/System.hpp"

// #include "includes/utility/NotImplementedError.hpp"
#include "includes/utility/NotSupportedError.hpp"

std::weak_ptr<Component> System::create_component(int64_t uuid) {
  throw NotSupportedError{};
}
std::shared_ptr<Entity> System::create_entity(const std::string &name,
                                              int64_t uuid) {
  throw NotSupportedError{};
}
bool System::remove(Component *c) { throw NotSupportedError{}; }
bool System::remove(Entity *e) { throw NotSupportedError{}; }
