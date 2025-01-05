#pragma once

#include "includes/system/Storage.hpp"
#include "includes/utility/VariadicTable.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <filesystem>
#include <vector>
#include <format>
#include <string>

#ifndef SHADER_ABSOLUTE_PATH
#define SHADER_ABSOLUTE_PATH "shaders/"
#endif

#ifndef ASSET_ABSOLUTE_PATH
#define ASSET_ABSOLUTE_PATH "assets/"
#endif

// abs -> UI
// rel -> big json

struct AssetManager : public Storage<std::filesystem::path> {
  const std::string get_name() const override { return "Asset Manager"; }

  explicit AssetManager(UUIDManager *um) : Storage(um) {
    LOG(std::format("created {}", get_name()));
  }

  //   enum class Type { Shader, Material, Object, Unknown }; // maybe do this

  struct Asset {
    Asset(AssetManager *am, std::filesystem::path path) : _am{am} {
      set_path(path);
    }

    Asset(AssetManager *am, uuid id, std::filesystem::path path) : _am{am} {
      set(id, path);
    }

    // Type type;
    uuid _uuid = boost::uuids::nil_uuid();
    std::filesystem::path _path;

    void set(uuid id, std::filesystem::path path) { _am->set(id, path); }

    bool set_uuid(uuid id) {
      // check if uuid is known on manager
      // -> if not in manager, this uuid is invalid
      auto opath = _am->get(id);
      if (!opath.has_value()) {
        return false;
      }
      _uuid = id;
      _path = opath.value();
      return true;
    }

	/**
	 * 
	 */
    void set_path(std::filesystem::path path) {
      auto oid = _am->get(path);
      if (oid.has_value()) { // path already exists
        _uuid = oid.value();
      } else { // path doesn't exists -> create a new one
        auto id = _am->create(path);
        _uuid = id;
      }
      _path = path;
    }

    std::filesystem::path get_path() const { return _path; }

    // set_uuid()
    // -> _am->get_instance().get_uuid() -> true: good
    //                                   -> false: error

    // set_path()
    // -> _am->get_instance().get_path() -> true: replace uuid here
    //                                   -> false: create new uuid in AM
    // get_path() -> abs

  private:
    AssetManager *_am;
  };

  struct DefaultAssets {
    explicit DefaultAssets(AssetManager *am) : shader(am, std::filesystem::path(SHADER_ABSOLUTE_PATH) / "default_shader"),
                                      mtl(am, std::filesystem::path(ASSET_ABSOLUTE_PATH) / "default.mtl"),
                                      obj(am, std::filesystem::path(ASSET_ABSOLUTE_PATH) / "default.obj") {}
     Asset shader;
     Asset mtl;
     Asset obj;
  };

  inline void set(uuid id, std::filesystem::path path) {
    auto opath = get(id);
    if (opath.has_value()) {
      LOG_WARN(std::format("ID \"{}\" already exists!! This can result in inconsistent behaviour!!",
                           boost::uuids::to_string(id)));
    }
    _storage[id] = path;
  }

  inline uuid create(std::filesystem::path path) {
    auto id = _um->create(this);
    LOG(std::format("added new path \"{}\" with UUID \"{}\"", path.string(),
                    boost::uuids::to_string(id)));
    _storage[id] = path;
    return id;
  }

  bool remove(uuid id) override { return false; };

  void print() {
    VariadicTable<std::string, std::string> vt({"UUID", "Path"});

    for (const auto &[id, path] : _storage) {
      vt.addRow(boost::uuids::to_string(id), path.string());
    }

    // TODO: Use Log class as an option? -> Print tables to file
    vt.print(std::cout);
    std::cout << std::endl;
  }

private:
  // map<uuid, path> _storage; // part of base class
};
