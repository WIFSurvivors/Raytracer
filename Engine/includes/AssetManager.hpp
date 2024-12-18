#pragma once

#include <boost/uuid/uuid.hpp>
#include <filesystem>
#include <vector>

#ifndef SHADER_ABSOLUTE_PATH
#define SHADER_ABSOLUTE_PATH "shaders/"
#endif

#ifndef ASSET_ABSOLUTE_PATH
#define ASSET_ABSOLUTE_PATH "assets/"
#endif

struct AssetManager {
  using uuid = boost::uuids::uuid;

  enum class Type { Shader, Material, Object };

  struct Asset {
    uuid uuid;
    Type type;
    std::filesystem::path path;
  };

private:
  std::vector<Asset> _assets;
};
