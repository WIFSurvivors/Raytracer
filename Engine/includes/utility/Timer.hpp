#pragma once

#include <chrono>
#include <cstddef>

class Timer {
public:
  Timer();

  void update();
  inline float get_duration() const {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<float>(now - _start_time).count();
  }

private:
  /**
   * _start_time -> creation of this class
   * _t0 -> previous timestep
   * _t1 -> current/new timestep
   */
  std::chrono::high_resolution_clock::time_point _start_time, _t0, _t1;
};
