#include "includes/UUIDManager.hpp"
#include "UUIDManager.hpp"

int UUIDManager::getNewUUID() {
  auto new_uuid = _uuid_counter++;
  _uuid_system_mapping[new_uuid] = nullptr;
  return new_uuid;
}

int UUIDManager::getNewUUID(System *s) {
  auto new_uuid = _uuid_counter++;
  _uuid_system_mapping[new_uuid] = s;
  return new_uuid;
}