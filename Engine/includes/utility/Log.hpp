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
#include <chrono>

namespace RT {

#define LOG_ERROR(msg) Log::get_instance().print(msg, Log::Level::Error);
#define LOG_WARN(msg) Log::get_instance().print(msg, Log::Level::Warn);
#define LOG(msg) Log::get_instance().print(msg, Log::Level::Message);
#define LOG_DEBUG(msg) Log::get_instance().print(msg, Log::Level::Debug);
#define LOG_TEST(msg) Log::get_instance().print(msg, Log::Level::Test);
#define LOG_TCP(msg) Log::get_instance().print(msg, Log::Level::Tcp);
#define LOG_FRAME_DATA(msg)                                                    \
  Log::get_instance().print(msg, Log::Level::FrameData);
#define LOG_NEW_LINE(msg) Log::get_instance().new_line();
#define LOG_FILE_PATH Log::get_instance().get_absolute_log_file_path()

namespace fs = std::filesystem;

struct Log {
  inline static Log &get_instance() {
    static Log instance;
    return instance;
  }

  enum class Level {
    Error,
    Warn,
    Message,
    Debug,
	Test,
    Tcp,
    FrameData
  };

private:
  Log() = default;
  Log(const Log &) = delete;
  Log &operator=(const Log &) = delete;

  inline static std::mutex _buffer_mutex;

  Level _log_level = Level::Tcp;
  Level _file_level = Level::Tcp;

  fs::path _log_file_path;
  std::deque<std::string> _file_buffer;

  std::string get_current_time_ms_full();
  std::string get_current_time_ms();
  void write_to_buffer(const std::string &msq, const Level &level);

  std::string level_to_string(Level level);
  std::string level_to_ansi_color(Level level);
  std::string reset_color();

public:
  void print(const std::string &msq, const Level &level);
  inline void new_line() { std::cout << '\n'; }

  void init_file();
  inline void set_cout_log_level(Level l) { _log_level = l; }
  inline void set_file_log_level(Level l) { _file_level = l; }

  inline const std::string get_relative_log_file_path() const {
    return _log_file_path.string();
  }
  inline const std::string get_absolute_log_file_path() const {
    return fs::absolute(_log_file_path).string();
  }

  void clear_buffer();
  void display_color_demo();
};
} // namespace RT
