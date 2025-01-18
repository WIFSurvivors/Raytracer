#include "includes/UUIDManager.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include "includes/utility/VariadicTable.hpp"
#include "includes/utility/Log.hpp"
#include <iostream>
#include <boost/uuid/uuid_io.hpp>
#include <format>
#include <algorithm>

namespace RT {
UUIDManager::UUIDManager() { LOG("created UUID Manager"); }
UUIDManager::~UUIDManager() {
  LOG(std::format("destroyed UUID Manager!! Elements not removed prior: {}",
                  _uuid_storage_mapping.size()));
				  
  for (const auto &[id, sto] : _uuid_storage_mapping) {
	std::cout << sto->get_name() << "|" << boost::uuids::to_string(id) << std::endl;
	LOG(std::format("{}: {}", sto->get_name(), boost::uuids::to_string(id)));
  } 
  print();
}

bool UUIDManager::add(uuid id, IStorage *s) {
  if (_uuid_storage_mapping.contains(id)) {
    auto existing_name = _uuid_storage_mapping[id]->get_name();
    LOG_ERROR(std::format("Adding existing UUID {} to {}, but is part of {}",
                          boost::uuids::to_string(id), s->get_name(),
                          existing_name));
    return false;
  }
  _uuid_storage_mapping[id] = s;
  return true;
}

UUIDManager::uuid UUIDManager::create(IStorage *s) {
  auto new_uuid = gen();
  LOG(std::format("created uuid: {} for system {}",
                  boost::uuids::to_string(new_uuid), s->get_name()));
  _uuid_storage_mapping[new_uuid] = s;
  return new_uuid;
}

void UUIDManager::print() {
  VariadicTable<std::string, std::string> vt({"All UUIDs", "System Name"});

  for (const auto &[id, sto] : _uuid_storage_mapping) {
    vt.addRow(boost::uuids::to_string(id), sto->get_name());
  }

  // TODO: Use Log class as an option? -> Print tables to file
  vt.print(std::cout);
  std::cout << std::endl;
}

bool UUIDManager::remove(uuid id) {
  auto sys = get_storage(id);
  if (sys == nullptr) { // uuid doesn't exist
    return false;
  }
  return sys->remove(id);
}

bool UUIDManager::remove_without_system(uuid id) {
  if (_uuid_storage_mapping[id] == nullptr) {
    return false;
  }
  _uuid_storage_mapping.erase(id);
  LOG(std::format("removed uuid {}", boost::uuids::to_string(id)));
  return true;
}
} // namespace RT
