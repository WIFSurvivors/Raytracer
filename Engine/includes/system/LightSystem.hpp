#pragma once
#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/LightComponent.hpp"
#include "includes/system/System.hpp"
#include <boost/uuid/uuid.hpp>
#include <glm/glm.hpp>
#include <optional>
#include <memory>
#include <string>

struct LightSystem : public System<LightComponent>
{
  LightSystem();

  LightComponent *create_component(uuid id, Entity *e);
  LightComponent *create_component(uuid id, Entity *e, float intensity);
  LightComponent *create_component(uuid id, Entity *e, glm::vec3 color);
  LightComponent *create_component(uuid id, Entity *e, float intensity = 10.f, glm::vec3 color = glm::vec3{1.f, 1.f, 1.f});

  inline std::string get_system_name() const override {
    return "Light System";
  }

  void print() override;

private:

};
