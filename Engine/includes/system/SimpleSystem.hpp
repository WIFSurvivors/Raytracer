#pragma once
#include "includes/Entity.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/system/System.hpp"
// #include "includes/UUIDManager.hpp"
#include <boost/uuid/uuid.hpp>
#include <map>
#include <memory>
#include <string>

namespace RT {
struct UUIDManager;
struct FrameSnapshot;

struct SimpleSystem : public System<SimpleComponent> {
  explicit SimpleSystem(UUIDManager *um);

  SimpleComponent *create_component(Entity *e, int value);
  SimpleComponent *create_component(Entity *e, uuid id, int value);

  void update(const FrameSnapshot &snapshot);

  void print() override;

  inline const std::string get_name() const final { return "Simple System"; }

  // private:
  //   using System::create_component_base;
};
} // namespace RT
