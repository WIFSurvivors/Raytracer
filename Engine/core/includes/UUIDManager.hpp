#pragma once

#include "includes/utility/SimpleLogger.hpp"
#include "includes/system/System.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <map>

typedef boost::uuids::uuid uuid;

struct UUIDManager {
  UUIDManager();

  uuid getNewUUID();
  uuid getNewUUID(ISystem *s);

  ISystem *get_system(uuid id);

  void print();

private:
  boost::uuids::random_generator gen{};
  std::map<uuid, ISystem *> _uuid_system_mapping{};
};
