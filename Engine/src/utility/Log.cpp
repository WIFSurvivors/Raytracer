#include "includes/utility/Log.hpp"

void Log::print(const std::string &msq, const Level &level) {
  //   std::lock_guard<std::mutex> lock(mutex_);
  std::ostringstream logEntry;
  logEntry << level_to_ansi_color(level) << "-- " << msq << reset_color()
           << "\n";
  if (level <= _log_level) {
    if (level == Level::Error) {
      std::cerr << logEntry.view();
    } else {
      std::cout << logEntry.view();
    }
  }
  write_to_buffer(msq, level);
}

// ================== FILE ==========================================

void Log::init_file() {
    std::filesystem::path folder(ROOT_ABSOLUTE_PATH);
    fs::current_path(folder);
    if (fs::is_directory(folder / "log")) {
      fs::create_directory("log");
    }
    folder /= "log";
    Log::message(std::format("CURRENT_PATH: {}", folder.string()));

    std::string filename{};

    std::ofstream _logFile;
    _log_file_path = (folder / "hello.txt").string();
    _logFile.open(_log_file_path, std::ios::app);
    _logFile << "wawa2";
    _logFile.close();
}

void Log::write_to_buffer(const std::string &msq, const Level &level) {
  _file_buffer.push_back(std::format("[{}] {}", level_to_string(level), msq));
}

void Log::start_new_entry(int frame_count, float delta_time, float frame_start_time){
  _file_buffer.push_back(std::format("{} FRAME {} {}", std::string(10, '*'), frame_count, std::string(30, '*')));
  _file_buffer.push_back(std::format("Total runtime: {}", frame_start_time));
  _file_buffer.push_back(std::format("Frame delta time: {}", delta_time));
}

void Log::clear_buffer() {
  init_file();
  std::lock_guard<std::mutex> lock(mutex_);
  std::cout << "size: " << _file_buffer.size() << "\n";
  for (auto &str : _file_buffer) {
    std::cout << str << "\n";
  }
  std::cout << std::endl;
  _file_buffer.clear();
}

// ================== LOG LEVEL FUNCTION =====================================

std::string Log::level_to_string(Level level) {
  switch (level) {
  case Level::Error:
    return "ERROR";
  case Level::Warn:
    return "WARN";
    break;
  case Level::Message:
    return "MESSAGE";
    break;
  case Level::Debug:
    return "DEBUG";
    break;
  case Level::Tcp:
    return "TCP";
    break;
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
  return "<invalid level>";
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
  LOG_NEW_LINE();
}
