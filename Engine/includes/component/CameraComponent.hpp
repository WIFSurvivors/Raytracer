#pragma once

#include "includes/component/Component.hpp"

struct CameraSystem;
struct FrameSnapshot;

struct CameraComponent : public IComponent {
  CameraComponent(uuid id, Entity *e);
  CameraComponent(uuid id, Entity *e, float fov);

  void update(const FrameSnapshot& snapshot) override;

  inline void set_fov(const float fov) { _fov = fov; }
  inline float get_fov() const { return _fov; }
  
  inline bool is_main_camera() const { return _is_main_camera; }

  void set_from_json(boost::json::object obj) override;

private:
  bool _is_main_camera{false};
  float _fov{60.f};

  friend CameraSystem;
  inline void set_is_main_camera(bool value) { _is_main_camera = value; }
  
  boost::json::object to_json_details() const override;
};
