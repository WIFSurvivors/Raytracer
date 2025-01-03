#pragma once

#include "includes/UUIDManager.hpp"
#include <boost/uuid/uuid.hpp>
#include <optional>
#include <string>

/**
 * Defines Getter via UUID by returning an optional object pointer.
 * Internal storage can be adjusted according to each system
 */
template <class T> struct Storage : public IStorage {
  using uuid = boost::uuids::uuid;
  
  Storage() = default;
  virtual ~Storage() = default;

  /**
   * Get Object stored in this system. Will return std::nullopt when UUID is
   * not found.
   */
  virtual std::optional<T> get(uuid id) {
    return _storage.contains(id) ? std::make_optional(_storage[id])
                                 : std::nullopt;
  }

  /**
   * Removes Object from container by uuid.
   * This will also remove it's link to it's entity.
   */
  bool remove(uuid id) = 0;

private:
  std::map<uuid, T> _storage;
};
