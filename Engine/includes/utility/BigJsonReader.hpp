#pragma once

#include <filesystem>
#include "includes/Scene.hpp"

namespace RT {
struct Scene;
class BigJsonReader {
public:
  BigJsonReader() = default;
  virtual ~BigJsonReader() = default;

  void write_to_json(const std::filesystem::path filePath,
                     std::unique_ptr<Scene> s);

  std::optional<std::unique_ptr<Scene>>
  read_from_json(const std::filesystem::path filePath, Engine *engine);

private:
};
} // namespace RT
