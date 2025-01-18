#pragma once

#include "includes/tcp_server/TcpCommand.hpp"
#include "includes/Engine.hpp"
#include "includes/utility/Log.hpp"
#include "includes/utility/NotImplementedError.hpp"

#define UNDO_COMMAND "Undo"

using RT::Engine;
using RT::Log;
class UndoCommand : public TcpCommand
{
public:
    UndoCommand(int number = 0) : _number(number) {}
    std::string execute(Engine *engine);
    int get_number() { return _number; }
    private:
    int _number;
};
