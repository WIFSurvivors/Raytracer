#pragma once

// #include "includes/system/System.hpp"
// #include "includes/utility/Log.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <map>

struct UUIDManager;

/**
 * Base class for any uuid to storage type binding. This is template-less to
 * allow for more flexible usage. Every Storage unit knows a uuid manager, where
 * it will register newly created entries!
 */
struct IStorage {
  using uuid = boost::uuids::uuid;
  virtual ~IStorage() = default;
  virtual const std::string get_name() const = 0;
};

struct UUIDManager {
  using uuid = boost::uuids::uuid;
  UUIDManager();

  uuid create_uuid(IStorage *s);

  inline IStorage *get_storage(uuid id) { return _uuid_storage_mapping[id]; }

  /**
   * This only removes an entity from the storage. If the associated Componet
   * oder Entity hasn't been removed before, it will not be accessible anymore!
   */
  bool remove(uuid id); // TBD

  void print();

private:
  boost::uuids::random_generator gen{};
  std::map<uuid, IStorage*> _uuid_storage_mapping{};
};
