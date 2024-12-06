#pragma once

#include "includes/system/System.hpp"
#include "includes/system/SimpleSystem.hpp"
#include "includes/system/EntitySystem.hpp"
#include "includes/system/RenderSystem.hpp"
#include "includes/system/CameraSystem.hpp"
#include "includes/UUIDManager.hpp"
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <string>

typedef boost::uuids::uuid uuid;
struct Entity;
struct Engine;
struct SimpleSystem;
struct EntitySystem;

struct Scene {
  explicit Scene(Engine *e);
  Scene(Engine *e, uuid id);
  ~Scene();

  std::weak_ptr<Entity> get_root();

  std::shared_ptr<Entity> create_entity(const std::string &name);
  std::shared_ptr<Entity> create_entity(const std::string &name, uuid id);
  std::shared_ptr<Entity> create_entity(const std::string &name,
                                        std::shared_ptr<Entity> parent);
  std::shared_ptr<Entity> create_entity(const std::string &name, uuid id,
                                        std::shared_ptr<Entity> parent);

  void print();

  void generate_sample_content();

  inline UUIDManager *get_uuid_manager() { return &_uuid_manager; }
  inline RenderSystem *get_render_system() { return &_render_system; }

  void update(float dt);

private:
  std::shared_ptr<Entity> create_root(const std::string &name);
  std::shared_ptr<Entity> create_root(const std::string &name, uuid id);

  EntitySystem _entity_system{};
  SimpleSystem _simple_system{};
  CameraSystem _camera_system{};
  RenderSystem _render_system;

  UUIDManager _uuid_manager{};

  std::shared_ptr<Entity> _root;
};
