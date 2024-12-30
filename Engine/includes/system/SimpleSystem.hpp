#pragma once
#include "includes/Entity.hpp"
#include "includes/component/SimpleComponent.hpp"
#include "includes/system/System.hpp"
#include <boost/uuid/uuid.hpp>
#include <map>
#include <memory>
#include <string>

struct FrameSnapshot;

struct SimpleSystem : public System<SimpleComponent> {
  SimpleSystem();

  SimpleComponent *create_component(uuid id, Entity *e, int value);

  void update(const FrameSnapshot& snapshot);

  void print() override;

  inline std::string get_system_name() const override {
    return "Simple System";
  }

private:
  using System::create_component_base;
  using typename System::uuid;
};
