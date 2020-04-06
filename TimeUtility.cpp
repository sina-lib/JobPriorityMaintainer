#include "TimeUtility.h"

std::chrono::system_clock::time_point getCurrentDayTime(void)
{
	using Hours = std::chrono::hours;
	auto now = std::chrono::system_clock::now();
	auto hib = std::chrono::system_clock::to_time_t(std::chrono::floor<std::chrono::hours>(now));

	hib = hib - (hib % (24 * 3600));

	return std::chrono::system_clock::from_time_t(hib);
}
