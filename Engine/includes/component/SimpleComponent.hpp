#pragma once

#include "includes/component/Component.hpp"
#include "includes/Entity.hpp"

struct FrameSnapshot;

struct SimpleComponent : public IComponent {
  SimpleComponent(uuid id, Entity *e);
  SimpleComponent(uuid id, Entity *e, int value);

  void update(const FrameSnapshot& snapshot) override;

  inline void set_value(int value) { _value = value; }
  inline int get_value() const { return _value; }
  void set_from_json(boost::json::object obj) override;

protected:
  boost::json::object to_json_details() const override;
  
private:
  int _value{};
};
