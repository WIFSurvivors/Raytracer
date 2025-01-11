#pragma once
#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/CameraComponent.hpp"
#include "includes/system/System.hpp"
#include <boost/uuid/uuid.hpp>
#include <optional>
#include <memory>
#include <string>

namespace RT {
/**
 * Handles currently active main camera, which is used by RenderSystem.
 */
struct CameraSystem : public System<CameraComponent> {
  explicit CameraSystem(std::shared_ptr<UUIDManager> um);

  CameraComponent *create_component(Entity *e, uuid id, float fov = 60.f);
  CameraComponent *create_component(Entity *e, float fov = 60.f);

  void set_main_camera(CameraComponent *cc);
  inline CameraComponent *get_main_camera() const { return _main_camera; }

  void sample_update_move_main_camera(float dt);

  inline const std::string get_name() const final { return "Camera System"; }

  /*
   * Only allows you to remove non-main-camera components.
   */
  bool remove(uuid id) override;
  void enforce_main_camera_deletion();
  void print() override;

private:
  CameraComponent *_main_camera = nullptr;
};
} // namespace RT
