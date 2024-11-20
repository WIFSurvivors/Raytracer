#pragma once

#include "includes/System.hpp"
#include "includes/SimpleSystem.hpp"
#include "includes/EntitySystem.hpp"
#include "boost/uuid/uuid.hpp"
#include <memory>
#include <string>

typedef boost::uuids::uuid uuid;
struct Entity;

struct Scene {
  Scene();

  std::weak_ptr<Entity> get_root();

  std::shared_ptr<Entity> create_entity(std::string name);
  std::shared_ptr<Entity> create_entity(std::string name, uuid id);
  std::shared_ptr<Entity> create_entity(std::string name,
                                        std::shared_ptr<Entity> parent);
  std::shared_ptr<Entity> create_entity(std::string name, uuid id,
                                        std::shared_ptr<Entity> parent);

  void print();

private:
  std::shared_ptr<Entity> _root{};

  UUIDManager *_uuid_manager;

  SimpleSystem simple_system{};
  EntitySystem entity_system{};
  WindowManager _wm{};
  core::RenderSystem render_sys{&_wm};
};
