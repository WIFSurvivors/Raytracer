#include "includes/system/CameraSystem.hpp"
#include "includes/utility/Log.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <format>
#include <iostream>
#include <iomanip> // for std::setprecision
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
// #include "glm/vec3.hpp"
#include "glm/ext.hpp"

CameraSystem::CameraSystem(UUIDManager *um) : System{um} {
  LOG("created camera system");
}

CameraComponent *CameraSystem::create_component(uuid id, Entity *e, float fov) {
  LOG("create camera component");
  auto c = create_component_base(id, e);
  c->set_fov(fov);
  if (!_main_camera) {
    set_main_camera(c);
  }
  return c;
}

void CameraSystem::set_main_camera(CameraComponent *cc) {
  if (_main_camera != nullptr) {
    _main_camera->set_is_main_camera(false);
  }
  _main_camera = cc;
  _main_camera->set_is_main_camera(true);
  LOG(std::format(
      "Set new main camera ({})\n\t-> camera is on entity \"{}\"({})",
      boost::uuids::to_string(_main_camera->get_uuid()),
      _main_camera->get_entity()->get_name(),
      boost::uuids::to_string(_main_camera->get_entity()->get_uuid())));
}

void CameraSystem::sample_update_move_main_camera(float t1) {
  if (!_main_camera)
    return;

  // this is btw a very non-optimal way of moving an object
  // ideally, we store the inital position *somewhere* for this type of movement

  auto ent = _main_camera->get_entity();
  auto pos = ent->get_local_position();

  auto dt_sin = std::sin(t1 * 2.5) * 10;
  pos.y = dt_sin;
  ent->set_local_position(pos);
}

void CameraSystem::print() {
  VariadicTable<std::string, bool, float, std::string> vt(
      {"CameraComponent UUID", "Main?", " FoV ", "Entity Name"});

  for (const auto &[key, value] : _components) {
    vt.addRow(boost::uuids::to_string(key), value->is_main_camera(),
              value->get_fov(), value->get_entity()->get_name());
  }

  std::streamsize defaultPrecision = std::cout.precision();
  std::cout << std::setprecision(3) << std::boolalpha;
  vt.print(std::cout);
  std::cout << std::setprecision(defaultPrecision) << std::endl;
  std::cout.unsetf(std::ios::boolalpha);
}
