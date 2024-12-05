#pragma once
#include "includes/TcpCommand.hpp"
#include <iostream>
#include <string>
#include <boost/uuid/uuid.hpp>
#define MOVE_COMMAND "MoveCommand"
class TcpCommand;

class MoveCommand : public TcpCommand {
public:
    MoveCommand(uuid_t uuid, int new_x, int new_y, int new_z) : TcpCommand(uuid), _new_x(new_x), _new_y(new_y), _new_z(new_z) {}
    std::string execute(Engine * engine) override;
    int undo() override;
private:
    int _old_x;
    int _old_y;
    int _old_z;
    int _new_x;
    int _new_y;
    int _new_z;
};
