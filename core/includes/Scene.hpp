#pragma once

#include <memory>
#include <string>
#include "includes/System.hpp"
#include "includes/SimpleSystem.hpp"
#include "includes/EntitySystem.hpp"

struct Entity;

static int64_t entity_counter = 0;

struct Scene {
  Scene();

  std::weak_ptr<Entity> get_root();

  std::shared_ptr<Entity> create_entity(std::string name);
  std::shared_ptr<Entity> create_entity(std::string name, int64_t uuid);
  std::shared_ptr<Entity> create_entity(std::string name,
                                        std::shared_ptr<Entity> parent);
  std::shared_ptr<Entity> create_entity(std::string name, int64_t uuid,
                                        std::shared_ptr<Entity> parent);

  void print();

private:
  std::shared_ptr<Entity> _root{};

  SimpleSystem simple_system{};
  EntitySystem entity_system{};
  /*RenderSystem render_system{};*/
};
