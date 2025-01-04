#pragma once

#include "includes/UUIDManager.hpp"
#include "includes/utility/Log.hpp"
#include <boost/uuid/uuid.hpp>
#include <functional>
#include <optional>
#include <string>

/**
 * Defines Getter via UUID by returning an optional object pointer.
 * Internal storage can be adjusted according to each system
 */
template <class T> struct Storage : public IStorage {
  using uuid = boost::uuids::uuid;
  
  Storage(UUIDManager *um) : IStorage(um) { /*LOG(std::format("created {}", get_name()));*/ }
  virtual ~Storage() = default;

  inline virtual std::optional<uuid> get(T obj){
	auto it = std::find_if(_storage.begin(), _storage.end(), [obj](const auto &val) {return val.second == obj;});
	if(it == _storage.end()) return {};
	return std::make_optional<uuid>(it->first);
  }

  /**
   * Get Object stored in this system. Will return std::nullopt when UUID is
   * not found.
   */
  inline virtual std::optional<T> get(uuid id) {
    return _storage.contains(id) ? std::make_optional(_storage[id])
                                 : std::nullopt;
  }

  /**
   * Removes Object from container by uuid.
   * This will also remove it's link to it's entity.
   */
  virtual bool remove(uuid id) = 0;

protected:
  std::map<uuid, T> _storage;
};
