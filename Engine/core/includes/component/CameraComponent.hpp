#include "includes/component/Component.hpp"

struct CameraComponent : public Component {
  CameraComponent(uuid id, Entity *e);
  CameraComponent(uuid id, Entity *e, float fov);

  void init() override;
  void update(const float dt) override;
  void destroy() override;

  inline void set_fov(const float fov) { _fov = fov; }
  inline float get_fov() const { return _fov; }

private:
  float _fov{60.f};
};