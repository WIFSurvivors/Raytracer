#pragma once
#include <stdexcept>

// Idea for this implementation here:
// https://stackoverflow.com/questions/24469927/does-c-have-an-equivalent-to-nets-notimplementedexception

class NotImplementedError : public std::logic_error {
public:
  NotImplementedError();

private:
};
