#pragma once

#include <boost/json.hpp>

struct JSONConvertable {
  virtual ~JSONConvertable() = default;

  virtual boost::json::object to_json() = 0;

  // dunno how to do as of right meow :)
  // virtual boost::json::object from_json() = 0;
};
