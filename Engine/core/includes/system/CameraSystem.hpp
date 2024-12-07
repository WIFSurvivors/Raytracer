#pragma once
#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/CameraComponent.hpp"
#include "includes/system/System.hpp"
#include <boost/uuid/uuid.hpp>
#include <optional>
#include <memory>
#include <map>

typedef boost::uuids::uuid uuid;

struct CameraSystem : public System {
  CameraSystem();

  CameraComponent *create_component(uuid id, Entity *e) override;
  CameraComponent *create_component(uuid id, Entity *e, float fov);
  bool remove(Component *c) override;
  bool remove(uuid uuid) override;

  inline const auto &get_components() const { return _components; }
  std::optional<Component *> get_component(uuid id);

  void set_main_camera(CameraComponent *cc);
  inline CameraComponent *get_main_camera() const { return _main_camera; }

  void print();

  void sample_update_move_main_camera(float dt);

private:
  std::map<uuid, std::unique_ptr<CameraComponent>> _components{};
  CameraComponent *_main_camera = nullptr;
};
