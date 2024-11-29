#pragma once

#include <iostream>
#include <string>

class SimpleLogger {
public:
  static void print(const std::string &s) { std::cout << s << std::endl; }
  static void print(const char *c) { std::cout << c << std::endl; }

private:
};
