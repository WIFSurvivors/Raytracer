#include "includes/system/System.hpp"
#include "includes/utility/NotSupportedError.hpp"
#include "includes/utility/NotImplementedError.hpp"

std::optional<Component *> System::get_component(uuid id) {}

Component *System::create_component(uuid uuid, Entity *e) {
  throw NotSupportedError{};
}

std::shared_ptr<Entity> System::create_entity(const std::string &name,
                                              uuid uuid) {
  throw NotSupportedError{};
}

std::optional<Component *> System::get_component(uuid id) {
  throw NotSupportedError{};
}

std::optional<Entity *> System::get_entity(uuid id) {
  throw NotSupportedError{};
}

bool System::remove(Component *c) { throw NotSupportedError{}; }
bool System::remove(Entity *e) { throw NotSupportedError{}; }


void System::print() {
  throw NotImplementedError{};
}
