#include "TimeUtility.h"
#include <iostream>

//#define __SHOW_DEBUG__

std::chrono::system_clock::time_point getCurrentDayTime(void)
{
	auto now = std::chrono::system_clock::now();
	return getDayTime(now);
}

std::chrono::system_clock::time_point getDayTime(std::chrono::system_clock::time_point begin)
{
	using Hours = std::chrono::hours;
	auto now = begin;

	now =  now + Hours(1);
	auto hix = std::chrono::system_clock::to_time_t(now);
#ifdef  __SHOW_DEBUG__
	std::cout << "before rounding: " << std::ctime(&hix) << std::endl;
#endif 
    auto hib = std::chrono::system_clock::to_time_t(std::chrono::floor<std::chrono::hours>(now));

    //std::cout << "change: " << (hib % (24*3600)) << std::endl;
	hib = hib - (hib % (24 * 3600));
	hib -= 3600; // seriously, why?
#ifdef  __SHOW_DEBUG__
	std::cout << "rounded: " << std::ctime(&hib) << std::endl;
#endif
	return std::chrono::system_clock::from_time_t(hib);
}


void testGetCurrentDayTimeFunction(void)
{
	auto now = std::chrono::system_clock::now();
	std::time_t s;
	const int ForwardOffset = 17;
	
	std::cout << "beginning current day test with offset : " << ForwardOffset << " mins" << std::endl;
	for (size_t i{}; i < 85; i++ , now += std::chrono::minutes(23))
	{
		s = std::chrono::system_clock::to_time_t(now);
		std::string buf{};
		buf = std::ctime(&s);
		buf.erase(buf.end()-1);
		std::cout << ">" << buf << "   -->    " ;
		s = std::chrono::system_clock::to_time_t(getDayTime(now));
		buf = std::ctime(&s);
		buf.erase(buf.end()-1);
		std::cout << buf << std::endl;;
	}
}

// ------------------------- Time Utilitu -----------------------------
