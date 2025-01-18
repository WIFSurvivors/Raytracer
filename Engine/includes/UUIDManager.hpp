#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <memory>
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

  explicit IStorage(std::shared_ptr<UUIDManager> um) : _um{um} {}
  virtual ~IStorage() = default;
  virtual const std::string get_name() const = 0;

  virtual bool remove(uuid id) = 0;

protected:
  std::shared_ptr<UUIDManager> _um;
};

/**
 * UUIDManager generates every UUID and links it to a Storage object. When
 * knowing a UUID, this class can be used to get the responsible storage unit
 * for further steps.
 */
struct UUIDManager {
  using uuid = boost::uuids::uuid;
  UUIDManager();
  ~UUIDManager();

  bool add(uuid id, IStorage *s);
  uuid create(IStorage *s);
  inline IStorage *get_storage(uuid id) { return _uuid_storage_mapping[id]; }

  /**
   * Removes UUID from Storage and executes the required clean-up inside the
   * system.
   */
  bool remove(uuid id);
  
  /**
   * Removes UUID from Storage. Call this, if the storage container is trying
   * to remove the UUID itself. This will prevent cyclic calls! 
   */
  bool remove_without_system(uuid id);

  void print();

private:
  boost::uuids::random_generator gen{};
  std::map<uuid, IStorage *> _uuid_storage_mapping{};
};
} // namespace RT
