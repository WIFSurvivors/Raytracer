#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <iostream>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <glm/vec3.hpp>

#define MOVE_COMMAND "MoveCommand"
class TcpCommand;

class MoveCommand : public TcpCommand {
public:
    MoveCommand(uuid uuid, float new_x, float new_y, float new_z) : TcpCommand(uuid), _new_position(new_x, new_y, new_z) {}
    std::string execute(Engine * engine) override;
    int undo() override;
private:
    float _old_x;
    float _old_y;
    float _old_z;
    glm::vec3 _new_position;
};
