
#define JobPM_VERSION_MAJOR 0
#define JobPM_VERSION_MINOR 5

#ifndef __TIME_UTILITY_HEADER__
#define __TIME_UTILITU_HEADER__
#include <chrono>


std::chrono::system_clock::time_point getDayTime(std::chrono::system_clock::time_point begin);
std::chrono::system_clock::time_point getCurrentDayTime(void);
void testGetCurrentDayTimeFunction(void);

#endif
