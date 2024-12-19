#pragma once
#include "includes/Entity.hpp"
#include "includes/component/Component.hpp"
#include "includes/component/CameraComponent.hpp"
#include "includes/system/System.hpp"
#include <boost/uuid/uuid.hpp>
#include <optional>
#include <memory>
#include <map>
#include <string>

typedef boost::uuids::uuid uuid;

/**
 * Handles currently active main camera, which is used by RenderSystem.
 */
struct CameraSystem : public System<CameraComponent> {
  CameraSystem();

  CameraComponent *create_component(uuid id, Entity *e, float fov = 60.f);

  void set_main_camera(CameraComponent *cc);
  inline CameraComponent *get_main_camera() const { return _main_camera; }

  void sample_update_move_main_camera(float dt);

  inline std::string get_system_name() const override {
    return "Camera System";
  }

  void print() override;

private:
  using typename System::uuid;

  CameraComponent *_main_camera = nullptr;
};
