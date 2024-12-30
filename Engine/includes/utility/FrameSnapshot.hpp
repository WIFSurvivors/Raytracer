#pragma once

class FrameSnapshot {
public:
  FrameSnapshot(float total_time, float delta_time, float accumulated_time,
           int frame_nr, int sub_frame_count);

  inline const float get_total_time() const { return _total_time; }
  inline const float get_delta_time() const { return _delta_time; }
  inline const float get_accumulated_time() const { return _accumulated_time; }

  inline const int get_frame_number() const { return _frame_nr; }
  inline const int get_sub_frame_count() const { return _sub_frame_count; }

private:
  float _total_time, _delta_time, _accumulated_time;
  int _frame_nr, _sub_frame_count;
};
