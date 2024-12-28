#pragma once

/* Modern C++ Singleton:
 * https://medium.com/@thomas_99791/how-to-write-a-singleton-class-in-c-51ed48bbef45
 */

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

  enum class Level : unsigned int { // change to bitset :)
    Error = 0,
    Warn = 1,
    Message = 2,
    Debug = 4,
    Tcp = 8
  };

private:
  Log() = default;
  Log(const Log &) = delete;
  Log &operator=(const Log &) = delete;

  inline static std::mutex mutex_;

  std::string _log_file_path;
  Level _log_level = Level::Tcp;
  std::deque<std::string> _file_buffer;

  void print(const std::string &msq, const Level &level);
  void write_to_buffer(const std::string &msq, const Level &level);

  std::string level_to_string(Level level);
  std::string level_to_ansi_color(Level level);
  std::string reset_color();

public:
  inline void message(const std::string &s) { print(s, Level::Message); }
  inline void warn(const std::string &s) { print(s, Level::Warn); }
  inline void error(const std::string &s) { print(s, Level::Error); }
  inline void debug(const std::string &s) { print(s, Level::Debug); }
  inline void tcp(const std::string &s) { print(s, Level::Tcp); }
  inline void new_line() { std::cout << '\n'; }

  void init_file();
  void set_log_level(Level level) { _log_level = level; }
  void start_new_entry(int frame_count, float delta_time, float total_time);
  void clear_buffer();
  void display_color_demo();
};
