#pragma once 
#include "includes/tcp_server/TcpCommand.hpp"
#include <iostream>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <glm/vec3.hpp>
#define ROTATE_COMMAND "RotateCommand"
class RotateCommand : public TcpCommand {
public:
    RotateCommand(uuid__ uuid,int new_x, int new_y, int new_z) : TcpCommand(uuid), _new_position(new_x, new_y, new_z) {}
    std::string execute(Engine * engine) override;
    int undo() override;
private:
int _old_x;
    int _old_y;
    int _old_z;
    glm::vec3 _new_position;
};
