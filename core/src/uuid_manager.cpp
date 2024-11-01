#include "uuid_manager.hpp"

int uuid_manager::getNewUUID() { return _uuid_counter++; }
