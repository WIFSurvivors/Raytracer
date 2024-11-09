#include "UUIDManager.hpp"

int UUIDManager::getNewUUID() { return _uuid_counter++; }
