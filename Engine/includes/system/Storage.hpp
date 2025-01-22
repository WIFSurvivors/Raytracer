#pragma once

#include "includes/UUIDManager.hpp"
#include "includes/utility/Log.hpp"
#include <boost/uuid/uuid.hpp>
#include <functional>
#include <optional>
#include <string>
#include <map>
#include <memory>

namespace RT {
/**
 * Defines Getter via UUID by returning an optional object pointer.
 * Internal storage can be adjusted according to each system
 */
template <class T> struct Storage : public IStorage {
  using uuid = boost::uuids::uuid;

  explicit Storage(std::shared_ptr<UUIDManager> um)
      : IStorage(um) { /*LOG(std::format("created {}", get_name()));*/
  }
  ~Storage() override {
    auto it = _storage.begin();
    while (it != _storage.end()) {
      _um->remove_without_system(it->first);
      it = _storage.erase(it);
    }
  }

  inline virtual std::optional<uuid> get(T obj) {
    auto it =
        std::find_if(_storage.begin(), _storage.end(),
                     [obj](const auto &val) { return val.second == obj; });
    if (it == _storage.end())
      return {};
    return std::make_optional<uuid>(it->first);
  }
  
  inline const std::map<uuid, T>& get_storage() const {
    return _storage;
  }

  /**
   * Get Object stored in this system. Will return std::nullopt when UUID is
   * not found.
   */
  inline virtual std::optional<T> get(uuid id) {
    return _storage.contains(id) ? std::make_optional(_storage[id])
                                 : std::nullopt;
  }

protected:
  std::map<uuid, T> _storage;
};
} // namespace RT
