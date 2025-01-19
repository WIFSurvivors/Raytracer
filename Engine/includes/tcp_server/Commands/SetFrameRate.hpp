#pragma once
#include "includes/tcp_server/TcpCommand.hpp"
#include <string>

#define SET_FRAME_RATE_COMMAND "SetFrameRate"

class SetFrameRate : public UndoableCommand {
public:
  explicit SetFrameRate(int frame_rate) : _frame_rate(frame_rate) {}
  std::string execute(RT::Engine *engine) override;
  std::string undo(RT::Engine *e) override;
  inline int get_bounces() const { return _frame_rate; }
  inline void set_bounces(int frame_rate) { _frame_rate = frame_rate; }

private:
  int _frame_rate;
  int _old_frame_rate;
};
