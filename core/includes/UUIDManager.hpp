#pragma once

#include "includes/System.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include <map>

typedef boost::uuids::uuid uuid;

struct UUIDManager {
  uuid getNewUUID();
  uuid getNewUUID(System *s);

  System *get_system(uuid id);

  void print();

private:
  boost::uuids::random_generator gen{};
  std::map<uuid, System *> _uuid_system_mapping{};
};
