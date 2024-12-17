#pragma once
#include "includes/TcpCommand.hpp"
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include "includes/CommandImplementations/MoveCommand.hpp"
#include "includes/CommandImplementations/RotateCommand.hpp"
#include "includes/ParsedTcpCommand.hpp"
#include "includes/CommandImplementations/GetRootCommand.hpp"
#include "includes/CommandImplementations/CloseRenderCommand.hpp"
#include "includes/CommandImplementations/GetChildEntitiesCommand.hpp"
#include "includes/CommandImplementations/GetComponentsCommand.hpp"
#include "includes/CommandImplementations/ImportJsonCommand.hpp"
#include "includes/CommandImplementations/GetEntityOptionsCommand.hpp"
#include "includes/CommandImplementations/ScaleCommand.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
struct TcpCommandFactory {
  TcpCommandFactory();
  std::unique_ptr<TcpCommand> create_command(ParsedTcpCommand parsed_command);

private:
};
