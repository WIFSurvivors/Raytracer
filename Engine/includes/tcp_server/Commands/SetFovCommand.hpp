#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define SET_FOV_COMMAND "SetFov"
class SetFovCommand : public TcpCommand
{
public:
    explicit SetFovCommand(float fov) : _fov(fov) {}
    std::string execute(Engine * engine) override;
    std::string undo() override;
    inline float get_bounces() const { return _fov; }
    inline void set_bounces(float fov) { _fov = fov; }
private:
    float _fov;

};
