#include "includes/system/LightSystem.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/VariadicTable.hpp"
#include <format>
#include <iomanip> // for std::setprecision

namespace RT {
LightSystem::LightSystem(std::shared_ptr<UUIDManager> um) : System{um} {
  LOG(std::format("created {}", get_name()));
}

LightComponent *LightSystem::create_component(Entity *e) {
  LOG("create camera component (a1)");
  auto c = create_component_base(e);
  return c;
}

LightComponent *LightSystem::create_component(Entity *e, uuid id) {
  LOG("create camera component (a2)");
  auto c = create_component_base(e, id);
  return c;
}

LightComponent *LightSystem::create_component(Entity *e, float intensity) {
  LOG("create camera component (b1)");
  auto c = create_component_base(e);
  c->set_intensity(intensity);
  return c;
}

LightComponent *LightSystem::create_component(Entity *e, uuid id,
                                              float intensity) {
  LOG("create camera component (b2)");
  auto c = create_component_base(e, id);
  c->set_intensity(intensity);
  return c;
}

LightComponent *LightSystem::create_component(Entity *e, glm::vec3 color) {
  LOG("create camera component (c1)");
  auto c = create_component_base(e);
  c->set_color(color);
  return c;
}

LightComponent *LightSystem::create_component(Entity *e, uuid id,
                                              glm::vec3 color) {
  LOG("create camera component (c2)");
  auto c = create_component_base(e, id);
  c->set_color(color);
  return c;
}

LightComponent *LightSystem::create_component(Entity *e, float intensity,
                                              glm::vec3 color) {
  LOG("create camera component (d1)");
  auto c = create_component_base(e);
  c->set_intensity(intensity);
  c->set_color(color);
  return c;
}

LightComponent *LightSystem::create_component(Entity *e, uuid id,
                                              float intensity,
                                              glm::vec3 color) {
  LOG("create camera component (d2)");
  auto c = create_component_base(e, id);
  c->set_intensity(intensity);
  c->set_color(color);
  return c;
}

void LightSystem::print() {
  VariadicTable<std::string, float, float, float, float> vt(
      {"LightComponent UUID", "Intensity", " Red ", "Green", " Blue "});

  for (const auto &[key, value] : _components) {
    vt.addRow(boost::uuids::to_string(key), value->get_intensity(),
              value->get_color().r, value->get_color().g, value->get_color().b);
  }

  std::streamsize defaultPrecision = std::cout.precision();
  std::cout << std::setprecision(3);
  vt.print(std::cout);
  std::cout << std::setprecision(defaultPrecision) << std::endl;
}
} // namespace RT
