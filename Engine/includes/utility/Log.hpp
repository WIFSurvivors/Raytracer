#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <cstdint>

struct Log {
  enum class Level : uint { Error = 0, Warn = 1, Message = 2, Debug = 3 };

private:
  static std::string level_to_ansi_color(Level level) {
    switch (level) {
    case Level::Error:
      return "\033[31m"; // red
    case Level::Warn:
      return "\033[33m"; // yellow
      break;
    case Level::Message:
      return "\033[39m"; // default
      break;
    case Level::Debug:
      return "\033[36m"; // cyan
      break;
    }
  }

  static std::string reset_color() {
    // set foreground and background colors to default
    return "\033[39m\033[49m";
  }

  static std::filesystem::path _log_file;
  static Level _log_level;
  static void print(const std::string &msq, const Level &level) {
    if (level <= _log_level) {
      std::cout << level_to_ansi_color(level) << msq << reset_color() << "\n";
    }
  }
  static void error(const std::string &msq, const Level &level) {
    std::cerr << level_to_ansi_color(level) << msq << reset_color() << "\n";
  }

public:
  static void message(const std::string &s) { print(s, Level::Message); }
  static void warn(const std::string &s) { print(s, Level::Warn); }
  static void error(const std::string &s) { error(s, Level::Error); }
  // disabled on release / active on verbose or sth
  static void debug(const std::string &s) { print(s, Level::Debug); }

  static void set_log_level(Level level) { _log_level = level; }
};
