#include "includes/UUIDManager.hpp"
#include "includes/utility/TablePrinter.hpp"
#include <iostream>
#include <boost/uuid/uuid_io.hpp>

uuid UUIDManager::getNewUUID() {
  auto new_uuid = gen();
  _uuid_system_mapping[new_uuid] = nullptr;
  return new_uuid;
}

uuid UUIDManager::getNewUUID(System *s) {
  auto new_uuid = gen();
  _uuid_system_mapping[new_uuid] = s;
  return new_uuid;
}

System *UUIDManager::get_system(uuid id) { return _uuid_system_mapping[id]; }

void UUIDManager::print() {
  // get longest character lengths in list
  size_t longest_id = 0;
  size_t longest_sys = std::string{"SYSTEM"}.length();
  auto compare = [](auto a, auto b) { return a > b ? a : b; };
  for (auto const &[id, sys] : _uuid_system_mapping) {
    auto size = boost::uuids::to_string(id).length();
    longest_id = compare(size, longest_id);
    size = std::string{typeid(sys).name()}.length();
    longest_sys = compare(size, longest_sys);
  }

  // print header SYSTEM | UUID
  std::string str = "UUID";
  size_t divider_length = str.size();
  TablePrinter::printHeader(str, str.length());
  std::cout << " | ";
  str = "SYSTEM";
  divider_length += str.size();
  TablePrinter::printHeader(str, str.length());

  // print divider =====|====; ("=|=" -> +3)
  std::cout << "\n" << std::string{"=", divider_length + 3} << "\n";

  // print each entry in dictionary
  for (auto const &[id, sys] : _uuid_system_mapping) {
    auto uuid_string = boost::uuids::to_string(id);
    TablePrinter::printElement(uuid_string, longest_id);
    std::cout << " | ";
    TablePrinter::printElement(std::string{typeid(sys).name()}, longest_sys);
    std::cout << "\n";
  }
  std::cout << std::endl;
}
