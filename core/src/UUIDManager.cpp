#include "includes/UUIDManager.hpp"
#include "includes/utility/TablePrinter.hpp"
#include <iostream>
#include <boost/uuid/uuid_io.hpp>
#include <format>

uuid UUIDManager::getNewUUID() {
  auto new_uuid = gen();
  _uuid_system_mapping[new_uuid] = nullptr;
  return new_uuid;
}

uuid UUIDManager::getNewUUID(System *s) {
  auto new_uuid = gen();
  SimpleLogger::print(std::format("-- created uuid: {} for system {}",
                                  boost::uuids::to_string(new_uuid),
                                  typeid(s).name()));
  _uuid_system_mapping[new_uuid] = s;
  return new_uuid;
}

System *UUIDManager::get_system(uuid id) { return _uuid_system_mapping[id]; }

void UUIDManager::print() {
  // get longest character lengths in list
  size_t longest_uuid = 0; // can probably be hardcoded for uuids...
  size_t longest_sys = std::string{"SYSTEM"}.length();
  auto compare = [](auto a, auto b) {
    return a > b ? a : b;
  }; // just use library function...
  for (auto const &[id, sys] : _uuid_system_mapping) {
    auto size = boost::uuids::to_string(id).length();
    longest_uuid = compare(size, longest_uuid);
    size = std::string{typeid(sys).name()}.length();
    longest_sys = compare(size, longest_sys);
  } // this can probably be simplified by a fancy library function
    // combination...

  // print header "UUID            | SYSTEM"
  std::string str = "UUID";
  size_t divider_length = str.size();
  TablePrinter::printElement(str, longest_uuid);
  std::cout << " | ";
  str = "SYSTEM";
  divider_length += str.size();
  TablePrinter::printElement(str, longest_sys);

  // print divider "=====|===="
  std::cout << "\n"
            << std::string(longest_uuid, '=')
            << "=|=" << std::string(longest_sys, '=') << "\n";

  // print each entry in dictionary
  for (auto const &[id, sys] : _uuid_system_mapping) {
    auto uuid_string = boost::uuids::to_string(id);
    TablePrinter::printElement(uuid_string, longest_uuid);
    std::cout << " | ";
    TablePrinter::printElement(std::string{typeid(sys).name()}, longest_sys);
    std::cout << "\n";
  }
  std::cout << std::endl;
}

UUIDManager::UUIDManager() { SimpleLogger::print("uuid manager 1"); }
