#pragma once

#include <filesystem>
#include <iostream>
#include <string>
#include <cstdint>
#include <cassert>

namespace fs = std::filesystem;
struct Log {
  enum class Level : unsigned int { Error = 0, Warn = 1, Message = 2, Debug = 3, Tcp = 4 };

private:
  static std::string level_to_ansi_color(Level level) {
    // ANSI-Coloring -> see:
    // https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
    switch (level) {
    case Level::Error:
      return "\033[91m"; // red
    case Level::Warn:
      return "\033[93m"; // yellow
      break;
    case Level::Message:
      return "\033[39m"; // default
      break;
    case Level::Debug:
      return "\033[96m"; // cyan
      break;
    case Level::Tcp:
      return "\033[92m"; // green
      break;
    }
    return Log::reset_color();
  }

  static std::string reset_color() {
    // set foreground and background colors to default
    return "\033[39m\033[49m";
  }

  static fs::path _log_file;
  static bool _init_log_file;
  static Level _log_level;

  static void print(const std::string &msq, const Level &level) {
    if (level <= _log_level) {
      std::cout << level_to_ansi_color(level) << "-- " << msq << reset_color()
                << "\n";
    }
  }
  static void error(const std::string &msq, const Level &level) {
    std::cerr << level_to_ansi_color(level) << "-- " << msq << reset_color()
              << "\n";
  }

  static void to_file() {
    if (!_init_log_file) {
      // https://en.cppreference.com/w/cpp/filesystem/create_directory
      // temp path? ->
      // https://en.cppreference.com/w/cpp/filesystem/temp_directory_path
      fs::current_path(fs::temp_directory_path());

      fs::create_directory("log");
      assert(!fs::create_directory("log"));

      _init_log_file = true;
    }
  }

public:
  static void message(const std::string &s) { print(s, Level::Message); }
  static void warn(const std::string &s) { print(s, Level::Warn); }
  static void error(const std::string &s) { error(s, Level::Error); }
  // disabled on release / active on verbose or sth... not done yet!!
  static void debug(const std::string &s) { print(s, Level::Debug); }
  static void tcp(const std::string &s) { print(s, Level::Tcp); }

  static void set_log_level(Level level) { _log_level = level; }

  static void display_color_demo() {
    message("Hello :3");
    warn("Hello :3");
    error("Hello :3");
    debug("Hello :3");
    tcp("Hello :3");
  }

  static void set_log_file(std::filesystem::path s) { _log_file = s; }
};
