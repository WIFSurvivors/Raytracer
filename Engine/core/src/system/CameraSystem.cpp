#include "includes/system/CameraSystem.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include "includes/utility/TablePrinter.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <format>
#include <iostream>
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
// #include "glm/vec3.hpp"
#include "glm/ext.hpp"

CameraSystem::CameraSystem() : ISystem{} {
  SimpleLogger::print("-- created camera system");
}

CameraComponent *CameraSystem::create_component(uuid id, Entity *e, float fov) {
  SimpleLogger::print("-- create camera component");
  auto c = create_component(id, e);
  c->set_fov(fov);
  return c;
}

void CameraSystem::set_main_camera(CameraComponent *cc) {
  if (_main_camera != nullptr) {
    _main_camera->set_is_main_camera(false);
  }
  _main_camera = cc;
  _main_camera->set_is_main_camera(true);
  SimpleLogger::print(std::format(
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

void CameraSystem::print_component(const CameraComponent &c) {
  TablePrinter::printElement("CameraComponent UUID", 36);
  std::cout << " | ";
  TablePrinter::printElement("fov", 12);
  std::cout << " | ";
  TablePrinter::printElement("is main camera", 14);
  std::cout << "\n";
  std::cout << std::string(36 + 12 + 14 + 2 * 3, '=');
  std::cout << "\n";
  for (auto const &[uuid, c] : _components) {
    std::cout << uuid << " | ";
    if (c == nullptr) {
      std::cout << "missing...\n";
      continue;
    }
    TablePrinter::printElement(c->get_fov(), 12);
    std::cout << " | ";
    TablePrinter::printElement(c->is_main_camera() ? " * " : "", 14);
    std::cout << "\n";
  }
  std::cout << std::endl;
}
