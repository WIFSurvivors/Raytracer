#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define SET_BOUNCES_COMMAND "SetBounces"

class SetBouncesCommand : public UndoableCommand {
public:
  explicit SetBouncesCommand(int bounces) : _bounces(bounces) {}
  std::string execute(RT::Engine *engine) override;
  std::string undo(RT::Engine *e) override;
  inline int get_bounces() const { return _bounces; }
  inline void set_bounces(int bounces) { _bounces = bounces; }

private:
  int _bounces;
  int _old_bounces;
};
