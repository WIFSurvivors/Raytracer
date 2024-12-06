#include "includes/system/CameraSystem.hpp"
#include "includes/utility/SimpleLogger.hpp"
#include <boost/uuid/uuid_io.hpp>

CameraSystem::CameraSystem() {
  SimpleLogger::print("-- CameraSystem initialized");
}

CameraComponent *CameraSystem::create_component(uuid id, Entity *e) {
  _components[id] = std::make_unique<CameraComponent>(id, e);
  auto ptr = _components[id].get();
  e->add_component(ptr);
  if (!_main_camera) {
    set_main_camera(ptr);
  }
  return ptr;
}

CameraComponent *CameraSystem::create_component(uuid id, Entity *e, float fov) {
  auto c = create_component(id, e);
  c->set_fov(fov);
  return c;
}

bool CameraSystem::remove(Component *c) { return remove(c->get_uuid()); }
bool CameraSystem::remove(uuid id) { return _components.erase(id); }

std::optional<CameraComponent *> CameraSystem::get_component(uuid id) {
  if (_components.contains(id)) {
    return std::make_optional(_components[id].get());
  }
  return {};
}

void CameraSystem::set_main_camera(CameraComponent *cc) {
  if (_main_camera) {
    _main_camera->set_is_main_camera(false);
  }
  _main_camera = cc;
  _main_camera->set_is_main_camera(true);
  SimpleLogger::print(std::format(
      "Set new main camera ({})\t-> camera is on entity \"{}\"({})",
      boost::uuids::to_string(_main_camera->get_uuid()),
      _main_camera->get_entity()->get_name(),
      boost::uuids::to_string(_main_camera->get_entity()->get_uuid())));
}
