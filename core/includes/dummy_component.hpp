#pragma once

#include "component.hpp"

struct dummy_component : component {
  dummy_component();
  virtual void init() override;
  virtual void update(float dt) override;
  virtual void destroy() override;
  virtual ~dummy_component() override;

private:
};
