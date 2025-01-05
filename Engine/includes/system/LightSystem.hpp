#pragma once
#include "includes/Entity.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/system/System.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/LightComponent.hpp"
#include <boost/uuid/uuid.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

struct LightSystem : public System<LightComponent> {
  explicit LightSystem(UUIDManager *um);

  LightComponent *create_component(Entity *e);
  LightComponent *create_component(Entity *e, uuid id);
  LightComponent *create_component(Entity *e, float intensity);
  LightComponent *create_component(Entity *e, uuid id, float intensity);
  LightComponent *create_component(Entity *e, glm::vec3 color);
  LightComponent *create_component(Entity *e, uuid id, glm::vec3 color);
  LightComponent *create_component(Entity *e, float intensity, glm::vec3 color);
  LightComponent *create_component(Entity *e, uuid id, float intensity,
                                   glm::vec3 color);

  inline const std::string get_name() const final {
    return "Light System";
  }

  const std::vector<glm::vec3> get_positions() const {
    std::vector<glm::vec3> positions;
    std::transform(_components.begin(), _components.end(),
                   std::back_inserter(positions),
                   [](auto &c) { return c.second->get_position(); });
    return positions;
  }

  const std::vector<glm::vec3> get_directions() const {
    std::vector<glm::vec3> directions;
    std::transform(_components.begin(), _components.end(),
                   std::back_inserter(directions),
                   [](auto &c) { return c.second->get_direction(); });
    return directions;
  }

  const std::vector<glm::vec3> get_colors() const {
    std::vector<glm::vec3> colors;
    std::transform(_components.begin(), _components.end(),
                   std::back_inserter(colors),
                   [](auto &c) { return c.second->get_color(); });
    return colors;
  }

  const std::vector<float> get_intensities() const {
    std::vector<float> intensities;
    std::transform(_components.begin(), _components.end(),
                   std::back_inserter(intensities),
                   [](auto &c) { return c.second->get_intensity(); });
    return intensities;
  }

  void print() override;

private:
};
