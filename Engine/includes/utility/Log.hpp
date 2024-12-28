#pragma once

#include <filesystem>
#include <iostream>
#include <format>
#include <string>
#include <cstdint>
#include <cassert>
#include <fstream>
#include <sstream>
#include <ctime>
#include <deque>
#include <mutex>

#ifndef ROOT_ABSOLUTE_PATH
#define ROOT_ABSOLUTE_PATH "/this_is_an_error/"
#endif

#ifndef LOG_ABSOLUTE_PATH
#define LOG_ABSOLUTE_PATH "/this_is_an_error/"
#endif

#define LOG(msg) Log::get_instance().message(msg);
#define LOG_WARN(msg) Log::get_instance().warn(msg);
#define LOG_ERROR(msg) Log::get_instance().error(msg);
#define LOG_DEBUG(msg) Log::get_instance().debug(msg);
#define LOG_TCP(msg) Log::get_instance().tcp(msg);
#define LOG_NEW_LINE(msg) Log::get_instance().new_line();

namespace fs = std::filesystem;
struct Log {
  inline static Log &get_instance() {
    static Log instance; // evil :C
    return instance;
  }

  enum class Level : unsigned int {
    Error = 0,
    Warn = 1,
    Message = 2,
    Debug = 3,
    Tcp = 4
  };

private:
  Log();
  Log(const Log &) = delete;
  Log &operator=(const Log &) = delete;

  inline static std::mutex mutex_;
  bool _init_log_file = false;
  std::string _log_file_path;
  Level _log_level = Level::Tcp;
  std::deque<std::string> _file_buffer;

  void print(const std::string &msq, const Level &level);

  std::string level_to_string(Level level);
  std::string level_to_ansi_color(Level level);
  std::string reset_color();

  void init_file();
  void write_to_buffer(const std::string &msq, const Level &level);

public:
  void message(const std::string &s) { print(s, Level::Message); }
  void warn(const std::string &s) { print(s, Level::Warn); }
  void error(const std::string &s) { print(s, Level::Error); }
  void debug(const std::string &s) { print(s, Level::Debug); }
  void tcp(const std::string &s) { print(s, Level::Tcp); }
  void new_line() { std::cout << '\n'; }

  void set_log_level(Level level) { _log_level = level; }

  void clear_buffer();

  void display_color_demo();
};
