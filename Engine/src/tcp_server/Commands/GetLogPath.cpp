#include "includes/tcp_server/Commands/GetLogPath.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"
std::string GetLogPath::execute(Engine *engine) {
  return LOG_FILE_PATH;
}
std::string GetLogPath::undo() { throw NotImplementedError{}; }
