#pragma once

#include <iostream>
#include <iomanip>

class TablePrinter {
public:
  static void new_line(){
	std::cout << '\n';
  }
  //   template <typename T> static void printHeader(T t, const int &width);
  template <typename T> static void printElement(T t, const int &width) {
    std::cout << std::left << std::setw(width) << std::setfill(_fill) << t
              << divider;
  }

  inline static const std::string divider = std::string(3, ' ');

private:
  static const char _fill = ' ';
};
