#pragma once

#include "includes/utility/SimpleLogger.hpp"
#include "includes/system/System.hpp"
#include "includes/Entity.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <map>

typedef boost::uuids::uuid uuid;

struct UUIDManager {
  UUIDManager();

  // i don't think this should *ever* be called...
  uuid create_uuid_ownerless();
  uuid create_uuid(ISystem *s);

  ISystem *get_system(uuid id);

  bool remove_uuid(uuid id);

  void print();

private:
  boost::uuids::random_generator gen{};
  // replace IComponent with IUUIDStorage at some point?
  std::map<uuid, ISystem *> _uuid_system_mapping{};
};
