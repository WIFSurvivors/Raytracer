#include "includes/UUIDManager.hpp"
#include "includes/utility/TablePrinter.hpp"
#include "includes/utility/VariadicTable.hpp"
#include <iostream>
#include <boost/uuid/uuid_io.hpp>
#include <format>
#include <algorithm>

UUIDManager::UUIDManager() { SimpleLogger::print("-- created UUID Manager"); }

uuid UUIDManager::create_uuid_ownerless() {
  auto new_uuid = gen();
  SimpleLogger::print(std::format(
      "-- created uuid: {} WITHOUT OWNER (most likely an entity) !!!!!",
      boost::uuids::to_string(new_uuid)));
  _uuid_system_mapping[new_uuid] = nullptr;
  return new_uuid;
}

uuid UUIDManager::create_uuid(ISystem *s) {
  auto new_uuid = gen();
  SimpleLogger::print(std::format("-- created uuid: {} for system {}",
                                  boost::uuids::to_string(new_uuid),
                                  typeid(s).name()));
  _uuid_system_mapping[new_uuid] = s;
  return new_uuid;
}

ISystem *UUIDManager::get_system(uuid id) { return _uuid_system_mapping[id]; }

void UUIDManager::print() {
  VariadicTable<std::string, std::string> vt(
      {"All UUID", "System Name"});

  for (const auto &[id, sys] : _uuid_system_mapping) {
    vt.addRow(boost::uuids::to_string(id),
              (sys ? sys->get_system_name() : "?Entity Storage?"));
  }

  vt.print(std::cout);
  std::cout << std::endl;
}

bool UUIDManager::remove_uuid(uuid id) { return false; }
