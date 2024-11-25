#pragma once

#include "includes/utility/SimpleLogger.hpp"
#include "includes/System.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include <map>

typedef boost::uuids::uuid uuid;

struct UUIDManager {
  UUIDManager();

  uuid getNewUUID();
  uuid getNewUUID(System *s);

  System *get_system(uuid id);

  void print();

private:
  boost::uuids::random_generator gen{};
  std::map<int, System *> _int_system_mapping{};
  std::map<uuid, System *> _uuid_system_mapping{};
};
