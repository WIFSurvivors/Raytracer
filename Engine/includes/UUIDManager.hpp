#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <map>
#include <string>

namespace RT {
struct UUIDManager;

/**
 * Base class for any uuid to storage type binding. This is template-less to
 * allow for more flexible usage. Every Storage unit knows a uuid manager, where
 * it will register newly created entries!
 * IStorage has been defined in UUIDManager.hpp as these parts are tightly
 * coupled and would produce a lot of #include Overhead without any real gain.
 */
struct IStorage {
  using uuid = boost::uuids::uuid;

  explicit IStorage(UUIDManager *um) : _um{um} {}
  virtual ~IStorage() = default;
  virtual const std::string get_name() const = 0;

protected:
  UUIDManager *_um;
};

/**
 * UUIDManager generates every UUID and links it to a Storage object. When
 * knowing a UUID, this class can be used to get the responsible storage unit
 * for further steps.
 */
struct UUIDManager {
  using uuid = boost::uuids::uuid;
  UUIDManager();

  bool add(uuid id, IStorage *s);
  uuid create(IStorage *s);
  inline IStorage *get_storage(uuid id) { return _uuid_storage_mapping[id]; }

  /**
   * This only removes an entity from the storage. If the associated Componet
   * oder Entity hasn't been removed before, it will not be accessible anymore!
   */
  bool remove(uuid id); // TBD

  void print();

private:
  boost::uuids::random_generator gen{};
  std::map<uuid, IStorage *> _uuid_storage_mapping{};
};
} // namespace RT
