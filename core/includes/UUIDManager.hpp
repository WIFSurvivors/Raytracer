#pragma once

#include "includes/System.hpp"
#include <map>

struct UUIDManager {
  int getNewUUID();

private:
  int _uuid_counter{1};
  std::map<int, System> _uuid_system_mapping{};
};
