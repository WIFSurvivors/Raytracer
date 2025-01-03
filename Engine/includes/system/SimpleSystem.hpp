#pragma once
#include "includes/Entity.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/system/System.hpp"
// #include "includes/UUIDManager.hpp"
#include <boost/uuid/uuid.hpp>
#include <map>
#include <memory>
#include <string>

struct UUIDManager;
struct FrameSnapshot;

struct SimpleSystem : public System<SimpleComponent> {
  SimpleSystem();

  SimpleComponent *create_component(uuid id, Entity *e, int value);

  void update(const FrameSnapshot& snapshot);

  void print() override;

  inline virtual const std::string get_name() const final {
    return "Simple System";
  }

private:
  using System::create_component_base;
};
