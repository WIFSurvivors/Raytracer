#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include "includes/tcp_server/Commands/MoveCommand.hpp"
#include "includes/tcp_server/Commands/RotateCommand.hpp"
#include "includes/tcp_server/Commands/ScaleCommand.hpp"
#include "includes/tcp_server/ParsedTcpCommand.hpp"
#include "includes/tcp_server/Commands/GetRootCommand.hpp"
#include "includes/tcp_server/Commands/CloseRenderCommand.hpp"
#include "includes/tcp_server/Commands/GetChildEntitiesCommand.hpp"
#include "includes/tcp_server/Commands/GetComponentsCommand.hpp"
#include "includes/tcp_server/Commands/GetEntityOptionsCommand.hpp"
#include "includes/tcp_server/Commands/ImportJsonCommand.hpp"
#include "includes/tcp_server/Commands/CreateEntityCommand.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
struct TcpCommandFactory {
  TcpCommandFactory();
  std::unique_ptr<TcpCommand> create_command(ParsedTcpCommand parsed_command);

private:
};
