#include "includes/UUIDManager.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include "includes/utility/VariadicTable.hpp"
#include "includes/utility/Log.hpp"
#include <iostream>
#include <boost/uuid/uuid_io.hpp>
#include <format>
#include <algorithm>

UUIDManager::UUIDManager() { LOG("created UUID Manager"); }

boost::uuids::uuid UUIDManager::create_uuid(IStorage *s) {
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
  throw NotImplementedError();
  return false;
}
