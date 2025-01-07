#include "includes/utility/BigJsonReader.hpp"

#include "includes/Scene.hpp"
#include "includes/utility/quicktype_json.hpp"
#include "includes/AssetManager.hpp"
#include "includes/utility/Log.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>

#include <filesystem>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <format>

std::string readFileToString(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + filePath);
  }

  std::ostringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

void writeStringToFile(const std::string &filePath,
                       const std::string &content) {
  std::ofstream file(filePath);
  if (!file.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + filePath);
  }

  file << content;

  if (!file) {
    throw std::ios_base::failure("Failed to write to file: " + filePath);
  }
}

void BigJsonReader::do_sth() {
  using namespace quicktype;
  auto test = std::filesystem::path(get_relative_json_folder_path() /
                                    "JsonParser_DummyFile.json");
  if (!std::filesystem::exists(test)) {
    LOG_WARN(std::format(
        "{} doesn't exist. Ignore this right now if you are not Jeb! :3",
        test.string()))
    return;
  }
  auto path = std::filesystem::absolute(test).string();
  std::cout << "PATH FOR JSON -> " << path << std::endl;
  auto str = readFileToString(path);
  std::cout << str << std::endl;
  writeStringToFile(path, str);
  QuicktypeJson data = nlohmann::json::parse(str);
}