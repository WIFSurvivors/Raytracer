#pragma once

#include "includes/UUIDManager.hpp"
#include "includes/AssetManager.hpp"
#include "includes/system/SimpleSystem.hpp"
#include "includes/system/EntityStorage.hpp"
#include "includes/system/RenderSystem.hpp"
#include "includes/system/CameraSystem.hpp"
#include "includes/system/LightSystem.hpp"
#include <boost/uuid/uuid.hpp>
#include <filesystem>
#include <memory>
#include <string>

namespace RT {
struct Entity;
struct Engine;
struct SimpleSystem;
struct EntityStorage;

struct FrameSnapshot;

struct Scene {
  using uuid = boost::uuids::uuid;

  /**
   * Construct a default scene and the required system, managers and so on.
   * Scene data can be filled from anywhere, but usually from BigJSON or TCPServer.
   * Root entity is the only thing, that exist for every scene.
   */
  explicit Scene(Engine *e);
  Scene(Engine *e, std::string title);
  virtual ~Scene();

  inline std::shared_ptr<UUIDManager> get_uuid_manager() {
    return _uuid_manager;
  }
  inline AssetManager *get_asset_manager() { return &_asset_manager; }
  inline EntityStorage *get_entity_storage() { return &_entity_storage; }
  inline RenderSystem *get_render_system() { return &_render_system; }
  inline SimpleSystem *get_simple_system() { return &_simple_system; }
  inline CameraSystem *get_camera_system() { return &_camera_system; }
  inline LightSystem *get_light_system() { return &_light_system; }

  inline std::weak_ptr<Entity> get_root() const { return _root; }
  inline std::optional<Entity *> get_entity(uuid id) {
    return _entity_storage.get(id);
  }
  std::optional<Entity *> operator[](uuid id) { return get_entity(id); }

  std::shared_ptr<Entity> create_entity(const std::string &name);
  std::shared_ptr<Entity> create_entity(const std::string &name, uuid id);
  std::shared_ptr<Entity> create_entity(const std::string &name,
                                        std::shared_ptr<Entity> parent);
  std::shared_ptr<Entity> create_entity(const std::string &name, uuid id,
                                        std::shared_ptr<Entity> parent);

  bool remove(Entity *e);
  bool remove(uuid id);

  // Ddvance the scene by updating every system.
  // This is currently only the render system.
  void update(const FrameSnapshot &snapshot);

  inline AssetManager::Asset create_asset(std::filesystem::path p) {
    return AssetManager::Asset(get_asset_manager(), p);
  }

  inline AssetManager::Asset create_asset(uuid id, std::filesystem::path p) {
    return AssetManager::Asset(get_asset_manager(), id, p);
  }
  
  void generate_test_scene();
  void print_system_data();

private:
  std::shared_ptr<Entity> create_root(const std::string &name);
  std::shared_ptr<Entity> create_root(const std::string &name, uuid id);

  std::string _title{"default"};

  std::shared_ptr<UUIDManager> _uuid_manager = std::make_shared<UUIDManager>();
  AssetManager _asset_manager{_uuid_manager};
  AssetManager::DefaultAssets _default_assets{&_asset_manager};
  EntityStorage _entity_storage{_uuid_manager};
  SimpleSystem _simple_system{_uuid_manager};
  CameraSystem _camera_system{_uuid_manager};
  LightSystem _light_system{_uuid_manager};
  RenderSystem _render_system;

  std::shared_ptr<Entity> _root;
};
} // namespace RT
