#include "TcpParser.hpp"

TcpParser::TcpParser() {}

int TcpParser::parse(std::string msg) {
  std::cout << "Got message: " << msg << std::endl;
  return 0;
}
