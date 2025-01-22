#include "includes/tcp_server/Commands/GetLogPath.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

using RT::Engine;
using RT::Log;

std::string GetLogPath::execute(Engine *engine) { 
      set_successfull(true);
    return LOG_FILE_PATH; }

