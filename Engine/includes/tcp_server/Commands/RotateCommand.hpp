#pragma once 
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <glm/vec3.hpp>
#define ROTATE_COMMAND "RotateCommand"
class RotateCommand : public TcpCommand {
public:
    RotateCommand(uuid uuid,int new_x, int new_y, int new_z) : TcpCommand(uuid), _new_rotation(new_x, new_y, new_z) {}
    std::string execute(Engine * engine) override;
    std::string undo() override;
    inline glm::vec3 get_new_rotation() const { return _new_rotation; }
    inline void set_new_rotation(glm::vec3 new_rotation) { _new_rotation = new_rotation; }
    inline glm::vec3 get_old_rotation() const { return _old_rotation; }
    inline void set_old_rotation(glm::vec3 old_rotation) { _old_rotation = old_rotation; }
private:
    glm::vec3 _old_rotation;
    glm::vec3 _new_rotation;
};
