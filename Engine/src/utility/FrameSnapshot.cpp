#include "includes/utility/FrameSnapshot.hpp"
#include "includes/utility/Log.hpp"
#include <format>

FrameSnapshot::FrameSnapshot(float total_time, float delta_time,
                             float accumulated_time, int frame_nr,
                             int sub_frame_count)
    : _total_time{total_time}, _delta_time{delta_time},
      _accumulated_time{accumulated_time}, _frame_nr{frame_nr},
      _sub_frame_count{sub_frame_count} {
  LOG_FRAME_DATA(std::format(
      "SNAPSHOT CREATED\n-total_time: {}\n-delta_time: "
      "{}\n-accumulated_time: {}\n-frame_nr: {}\n-sub_frame_count: {}",
      _total_time, _delta_time, _accumulated_time, _frame_nr,
      _sub_frame_count));
}
