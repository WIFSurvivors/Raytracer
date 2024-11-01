#pragma once

#include <map>

struct uuid_manager {
  int getNewUUID();

private:
  int _uuid_counter{1};
  //   std::map<int, system> _uuid_system_mapping{};
};
