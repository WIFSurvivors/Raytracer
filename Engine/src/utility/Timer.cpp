#include "includes/utility/Timer.hpp"

Timer::Timer(){
	_start_time = _t0 = _t1 = std::chrono::high_resolution_clock::now();
}
