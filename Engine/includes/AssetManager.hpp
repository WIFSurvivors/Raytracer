#pragma once

#include "includes/system/Storage.hpp"
#include "includes/utility/VariadicTable.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <filesystem>
#include <memory>
#include <vector>
#include <format>
#include <string>
#include <utility>

#ifndef SHADER_RELATIVE_PATH
#define SHADER_RELATIVE_PATH "shaders"
#endif

#ifndef ASSET_RELATIVE_PATH
#define ASSET_RELATIVE_PATH "assets"
#endif

#ifndef JSON_RELATIVE_PATH
#define JSON_RELATIVE_PATH "json"
#endif

namespace RT {
inline const fs::path get_relative_shader_folder_path() {
  return fs::path(SHADER_RELATIVE_PATH);
}

inline const fs::path get_absolute_shader_folder_path() {
  return fs::absolute(fs::path(get_relative_shader_folder_path()));
}

inline const fs::path get_relative_json_folder_path() {
  return fs::path(JSON_RELATIVE_PATH);
}

inline const fs::path get_absolute_json_folder_path() {
  return fs::absolute(fs::path(get_relative_json_folder_path()));
}

inline const fs::path get_relative_asset_folder_path() {
  return fs::path(ASSET_RELATIVE_PATH);
}

inline const fs::path get_absolute_asset_folder_path() {
  return fs::absolute(fs::path(get_relative_asset_folder_path()));
}

namespace fs = std::filesystem;
struct AssetManager : public Storage<fs::path> {
  const std::string get_name() const override { return "Asset Manager"; }

  explicit AssetManager(std::shared_ptr<UUIDManager> um) : Storage(um) {
    LOG(std::format("created {}", get_name()));
  }
  ~AssetManager() { LOG(std::format("destroyed {}", get_name())); }

  struct Asset {
    Asset(AssetManager *am, fs::path path, bool no_copy = false) : _am{am} {
      set_path(path, no_copy);
    }
    Asset(AssetManager *am, uuid id, fs::path path, bool no_copy = false)
        : _am{am} {
      set(id, path, no_copy);
    }

    fs::path get_path() const { return _path; }
    uuid get_uuid() const { return _uuid; }

    void set(uuid id, fs::path path, bool no_copy = false);
    bool set_uuid(uuid id);
    bool set_path(fs::path path, bool no_copy = false);

  private:
    AssetManager *_am;
    uuid _uuid = boost::uuids::nil_uuid();
    fs::path _path;
  };

  struct DefaultAssets {
    explicit DefaultAssets(AssetManager *am)
        : shader(am, get_relative_shader_folder_path() / "default_shader"),
          mtl(am, get_relative_asset_folder_path() / "default.mtl"),
          obj(am, get_relative_asset_folder_path() / "default.obj") {}
    Asset shader;
    Asset mtl;
    Asset obj;
  };

  std::optional<fs::path> set(uuid id, fs::path path, bool no_copy = false);
  std::optional<std::pair<uuid, fs::path>> create(fs::path path, bool no_copy = false);

  // remove is not supported in asset manager
  // storage container will be cleaned up on scene destruction
  bool remove(uuid id) override { return false; };

  void print();

private:
  bool can_open_file(const fs::path &path);
  bool is_path_in_subfolder(const fs::path &srcPath);
  std::optional<fs::path> copy_file_to_subfolder(const fs::path &srcPath);
  // to allow exporting scenes, every asset needs to be in the asset folder
  // if this is not the case, make a copy of the asset and store it in the asset
  // folder
  std::optional<fs::path> copy_file_and_replace_path(const fs::path &srcPath);
};
} // namespace RT
