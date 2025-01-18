#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define SET_FOV_COMMAND "SetFov"
class SetFovCommand : public UndoableCommand
{
public:
    explicit SetFovCommand(float fov) : _fov(fov) {}
    std::string execute(RT::Engine * engine) override;
    std::string undo(RT::Engine *e) override;
    inline float get_bounces() const { return _fov; }
    inline void set_bounces(float fov) { _fov = fov; }
private:
    float _fov;
    float _old_fov;
};
