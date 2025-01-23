#pragma once

#include "includes/component/Component.hpp"
#include "includes/Entity.hpp"
#include <glm/glm.hpp>

namespace RT {
struct LightSystem;
struct FrameSnapshot;

struct LightComponent : public IComponent {
  LightComponent(uuid id, Entity *e);

  void update(const FrameSnapshot &snapshot) override;

  // Shortcut function that accesses entities position, which acts as the lights
  // position.
  inline const glm::vec3 get_position() const {
    return get_entity()->get_world_position();
  }

  // Shortcut function that accesses entities rotation, which acts as the lights
  // direction.
  inline const glm::vec3 get_direction() const {
    return get_entity()->get_world_rotation();
  }

  inline const glm::vec3 get_color() const { return _color; }
  inline const float get_intensity() const { return _intensity; }

  inline void set_color(float r, float g, float b) {
    _color = glm::vec3{r, g, b};
  }
  inline void set_color(glm::vec3 c) { _color = c; }
  inline void set_intensity(float i) { _intensity = i; }

  void set_from_json(boost::json::object obj) override;

protected:
  boost::json::object to_json_details() const override;

private:
  friend LightSystem;
  glm::vec3 _color = glm::vec3{1.f, 1.f, 1.f}; // RGB
  float _intensity = 1.f;
};
} // namespace RT
