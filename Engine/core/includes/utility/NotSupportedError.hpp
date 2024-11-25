#pragma once
#include <stdexcept>

class NotSupportedError : public std::logic_error {
public:
  NotSupportedError();

private:
};
