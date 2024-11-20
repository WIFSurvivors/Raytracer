#pragma once

class TablePrinter {
public:
  template <typename T> static void printHeader(T t, const int &width);
  template <typename T> static void printElement(T t, const int &width);

private:
};
