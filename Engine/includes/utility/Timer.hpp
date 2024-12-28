#pragma once

#include <chrono>
#include <cstddef>

class Timer {
public:
  Timer();

  void update();
  inline float get_delta_time() const { return std::chrono::duration<float>(_t1 - _t0).count(); }
  inline float get_total_time() const { return std::chrono::duration<float>(_t1 - _start_time).count(); };
  inline size_t get_frame_count() const {return _frame_count;}

private:
/**
 * _start_time -> creation of this class
 * _t0 -> previous timestep
 * _t1 -> current/new timestep
 */
  std::chrono::high_resolution_clock::time_point _start_time, _t0, _t1;
  size_t _frame_count = 0;
};
