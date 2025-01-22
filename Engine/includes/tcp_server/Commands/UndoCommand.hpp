#pragma once

#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"
#include <string>

#define UNDO_COMMAND "Undo"

using RT::Engine;
using RT::Log;
class UndoCommand : public TcpCommand
{
public:
    explicit UndoCommand(int number = 1) : _number(number) {}
    std::string execute(Engine *engine);
    int get_number() { return _number; }
    private:
    int _number;
};
