#pragma once

#include "includes/component/Component.hpp"
// #include "includes/system/CameraSystem.hpp"

struct CameraSystem;

struct CameraComponent : public Component {
  CameraComponent(uuid id, Entity *e);
  CameraComponent(uuid id, Entity *e, float fov);

  void init() override;
  void update(const float dt) override;
  void destroy() override;

  inline void set_fov(const float fov) { _fov = fov; }
  inline float get_fov() const { return _fov; }

  inline bool is_main_camera() { return _is_main_camera; }

private:
  bool _is_main_camera{false};
  float _fov{60.f};
  
  friend CameraSystem;
  inline void set_is_main_camera(bool value) { _is_main_camera = value; }
};
