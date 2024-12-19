#pragma once

#include "includes/component/Component.hpp"
// #include "includes/system/CameraSystem.hpp"

struct CameraSystem;

struct CameraComponent : public IComponent {
  CameraComponent(uuid id, Entity *e);
  CameraComponent(uuid id, Entity *e, float fov);

  void update(const float dt) override;

  inline void set_fov(const float fov) { _fov = fov; }
  inline float get_fov() const { return _fov; }

  inline bool is_main_camera() { return _is_main_camera; }
  boost::json::object to_json_details() override;

protected:
  void to_json_details(boost::json::object obj) override;

private:
  bool _is_main_camera{false};
  float _fov{60.f};

  friend CameraSystem;
  inline void set_is_main_camera(bool value) { _is_main_camera = value; }
};
