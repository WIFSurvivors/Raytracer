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

/**
 * Handles currently active main camera, which is used by RenderSystem.
 */
struct CameraSystem : public System<CameraComponent> {
  CameraSystem();

  CameraComponent *create_component(uuid id, Entity *e, float fov);

  void set_main_camera(CameraComponent *cc);
  inline CameraComponent *get_main_camera() const { return _main_camera; }

  void sample_update_move_main_camera(float dt);

protected:
  // CameraComponent *create_component(uuid id, Entity *e);
  void print_component(const CameraComponent &c) override;

private:
  using System::create_component;

  CameraComponent *_main_camera = nullptr;
};
