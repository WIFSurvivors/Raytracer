#pragma once

#include "includes/component/Component.hpp"
#include "includes/Entity.hpp"

struct SimpleComponent : public IComponent {
  SimpleComponent(uuid id, Entity *e);
  SimpleComponent(uuid id, Entity *e, int value);

  void update(const float total_time) override;

  inline void set_value(int value) { _value = value; }
  inline int get_value() { return _value; }
  boost::json::object to_json_details() override;
  void set_from_json(boost::json::object obj) override;

protected:
  void to_json_details(boost::json::object obj) override;
  
private:
  int _value{};
};
