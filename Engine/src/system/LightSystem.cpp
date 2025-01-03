#include "includes/system/LightSystem.hpp"
#include "includes/UUIDManager.hpp"
#include "includes/utility/Log.hpp"
#include <format>
#include <iomanip> // for std::setprecision

LightSystem::LightSystem(UUIDManager *um) : System{um} {
  LOG(std::format("created {}", get_name()));
}

LightComponent *LightSystem::create_component(uuid id, Entity *e) {
  LOG("create camera component (a)");
  auto c = create_component_base(id, e);
  return c;
}

LightComponent *LightSystem::create_component(uuid id, Entity *e,
                                              float intensity) {
  LOG("create camera component (b)");
  auto c = create_component_base(id, e);
  c->set_intensity(intensity);
  return c;
}

LightComponent *LightSystem::create_component(uuid id, Entity *e,
                                              glm::vec3 color) {
  LOG("create camera component (c)");
  auto c = create_component_base(id, e);
  c->set_color(color);
  return c;
}

LightComponent *LightSystem::create_component(uuid id, Entity *e,
                                              float intensity,
                                              glm::vec3 color) {
  LOG("create camera component (d)");
  auto c = create_component_base(id, e);
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
