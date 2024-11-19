#pragma once

#include "includes/System.hpp"
#include <map>

struct UUIDManager {
  int64_t getNewUUID();
  int64_t getNewUUID(System *s);

  System *get_system(int64_t uuid);

private:
  int _uuid_counter{1};
  std::map<int, System *> _uuid_system_mapping{};
};
