#pragma once 
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>
#include <glm/vec3.hpp>
#define SCALE_COMMAND "ScaleCommand"
class ScaleCommand : public TcpCommand {
public:
    ScaleCommand(uuid uuid,int new_x, int new_y, int new_z) : TcpCommand(uuid), _new_scaling(new_x, new_y, new_z) {}
    std::string execute(Engine * engine) override;
    std::string undo() override;
    inline glm::vec3 get_new_scaling() const { return _new_scaling; }
    inline void set_new_scaling(glm::vec3 new_scaling) { _new_scaling = new_scaling; }
    inline glm::vec3 get_old_scaling() const { return _old_scaling; }
    inline void set_old_scaling(glm::vec3 old_scaling) { _old_scaling = old_scaling; }
private:
    glm::vec3 _old_scaling;
    glm::vec3 _new_scaling;

};
