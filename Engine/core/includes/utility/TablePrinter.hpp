#pragma once

#include <iostream>
#include <iomanip>

class TablePrinter {
public:
  //   template <typename T> static void printHeader(T t, const int &width);
  template <typename T> static void printElement(T t, const int &width) {
    std::cout << std::left << std::setw(width) << std::setfill(_separator) << t;
  }

private:
  static const char _separator = ' ';
};
