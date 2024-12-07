#pragma once

#include <boost/uuid/uuid.hpp>
#include <filesystem>
#include <string>
#include <map>

typedef boost::uuids::uuid uuid;

class FileManager {
public:
  FileManager() = default;
  virtual ~FileManager() = default;

private:
  std::map<uuid, std::filesystem::path> _files;
};
