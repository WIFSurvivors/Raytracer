#include "includes/utility/TablePrinter.hpp"

template <typename T> void TablePrinter::printHeader(T t, const int &width) {
  cout << middle << setw(width) << setfill(separator) << t;
}

template <typename T> void TablePrinter::printElement(T t, const int &width) {
  cout << left << setw(width) << setfill(separator) << t;
}