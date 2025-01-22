#pragma once

#include <filesystem>
#include "includes/Scene.hpp"
#include <memory>
#include <string>

namespace RT {
struct Scene;
class BigJson {
public:
  BigJson() = default;
  virtual ~BigJson() = default;

  std::optional<std::unique_ptr<Scene>>
  read_from_json(const std::filesystem::path filePath, Engine *engine);

  bool write_to_json(const std::filesystem::path filePath, Scene *s);

private:
  std::string readFileToString(const std::filesystem::path filePath);
  void write_string_to_path(const std::filesystem::path filePath,
                         const std::string &content);
};
} // namespace RT
