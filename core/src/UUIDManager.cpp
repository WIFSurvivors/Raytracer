#include "includes/UUIDManager.hpp"

int UUIDManager::getNewUUID() { return _uuid_counter++; }
