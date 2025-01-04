#include "includes/utility/Log.hpp"

void Log::print(const std::string &msg, const Level &level) {
  if (level <= _log_level) {
    std::ostringstream logEntry;
    logEntry << level_to_ansi_color(level) << "-- " << msg << reset_color()
             << "\n";
    if (level == Level::Error) {
      std::cerr << logEntry.view();
    } else {
      std::cout << logEntry.view();
    }
  }

  if (level <= _file_level) {
    write_to_buffer(msg, level);
  }
}

// ================== FILE ==========================================

void Log::init_file() {
  fs::path folder(ROOT_ABSOLUTE_PATH);
  std::cout << "PATH?? " << folder.string() << "\n";
  fs::current_path(folder);
  if (!fs::is_directory(folder / "log")) {
    fs::create_directory("log");
  }
  folder /= "log";
  auto file_name = get_current_time_ms_full() + "_LOG.txt";
  _log_file_path = (folder / file_name).string();
  std::cout << std::format("LOG PATH: {}", LOG_FILE_PATH) << "\n";

  std::ofstream _logFile;
  _logFile.open(_log_file_path, std::ios::app);
  _logFile << "Raytracer Engine -> Start of Log on "
           << get_current_time_ms_full() << "\n";
  _logFile.close();
}

std::string Log::get_current_time_ms_full() { // thanks chat-gpt <3
  auto now = std::chrono::system_clock::now();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                          now.time_since_epoch()) %
                      1000;
  std::time_t current_time = std::chrono::system_clock::to_time_t(now);
  std::tm *local_time = std::localtime(&current_time);

  std::ostringstream oss;
  oss << std::put_time(local_time, "%Y-%m-%d_%H-%M-%S") << "." << std::setfill('0')
      << std::setw(3) << milliseconds.count();
  return oss.str();
}

std::string Log::get_current_time_ms() { // thanks chat-gpt <3
  auto now = std::chrono::system_clock::now();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                          now.time_since_epoch()) %
                      1000;
  std::time_t current_time = std::chrono::system_clock::to_time_t(now);
  std::tm *local_time = std::localtime(&current_time);

  std::ostringstream oss;
  oss << std::put_time(local_time, "%H:%M:%S") << "." << std::setfill('0')
      << std::setw(3) << milliseconds.count();
  return oss.str();
}

void Log::write_to_buffer(const std::string &msg, const Level &level) {
  std::lock_guard<std::mutex> lock(_buffer_mutex);
  _file_buffer.push_back(std::format("[{}] {} | {}", get_current_time_ms(),
                                     level_to_string(level), msg));
}

void Log::clear_buffer() {
  std::lock_guard<std::mutex> lock(_buffer_mutex);
  if(_file_buffer.size() < 1) return;

  std::ofstream _logFile;
  _logFile.open(_log_file_path, std::ios::app);
  for (auto &str : _file_buffer) {
    _logFile << str << "\n";
  }
  _logFile.close();
  _file_buffer.clear();
}

// ================== LOG LEVEL FUNCTION =====================================

std::string Log::level_to_string(Level level) {
  switch (level) {
  case Level::Error:
    return "ERROR";
  case Level::Warn:
    return "WARN";
  case Level::Message:
    return "MESSAGE";
  case Level::Debug:
    return "DEBUG";
  case Level::Tcp:
    return "TCP";
  case Level::FrameData:
    return "FrameData";
  default:
    return "<invalid level>";
  }
}

std::string Log::level_to_ansi_color(Level level) {
  // ANSI-Coloring -> see:
  // https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
  switch (level) {
  case Level::Error:
    return "\033[91m"; // red
  case Level::Warn:
    return "\033[93m"; // yellow
  case Level::Message:
    return "\033[39m"; // default
  case Level::Debug:
    return "\033[96m"; // cyan
  case Level::Tcp:
    return "\033[92m"; // green
  case Level::FrameData:
    return "\033[90m"; // grey
  default:
    return "<invalid level>";
  }
}

std::string Log::reset_color() {
  // set foreground and background colors to default
  return "\033[39m\033[49m";
}

void Log::display_color_demo() {
  LOG("Hello :3");
  LOG_WARN("Hello :3");
  LOG_ERROR("Hello :3");
  LOG_DEBUG("Hello :3");
  LOG_TCP("Hello :3");
  LOG_FRAME_DATA("Hello :3");
  LOG_NEW_LINE();
}
