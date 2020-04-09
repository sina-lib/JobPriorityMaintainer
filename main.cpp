#include <iostream>
#include <chrono>
#include "JOB_holder.h"
#include <iomanip>
#include <chrono>
#include "Validator.hpp"
#include "TimeUtility.h"

inline void print(const std::string& matn)
{
	std::cout << matn << std::endl;
}
#include "JOB_test_helper.cpp"

int main(int argc, char* argv[])
{

	std::string name;
	std::chrono::system_clock::time_point dead {std::chrono::system_clock::now()};

	Validator<int> getValidInt{std::cin, std::cout};

	doSomeTests();
	return 0;
	print("save(s) or load(l)? ");
	char t;
	std::cin >> t;
	if (t=='s' || t=='S')
	{
		print("enter a name");
		std::cin >> name;

		auto ss = std::chrono::system_clock::to_time_t(dead);
		std::cout << "now is: " << std::ctime(&ss) << std::endl;
		print("how many days from now on? ");
		int val;
		//std::cin >> val;
		getValidInt >> val;
		dead+= std::chrono::hours(24*val);

		print("deadline time on that day(hour):");
		std::cin >> val;
		dead+= std::chrono::hours(val);

		print("constructed: ");
		std::time_t end = std::chrono::system_clock::to_time_t(dead);
		std::cout << std::ctime(&end) << std::endl;

		JOB_holder something{name, dead, 3.3};
		something.saveJob("rjobs/trivial01");
	}
	else
	{
		std::string nm{};
		print("enter the full name of that job: ");
		std::cin >> nm;
		JOB_holder something{nm};

		
		std::optional<std::chrono::minutes> rt = something.getRemainingTime();
		if (rt)
		{
			std::string nam, desc;
			something.getNameDescription(nam, desc);
			std::cout << "name of that job: " << nam << std::endl << "description of it: " << desc << std::endl; 
			std::cout << "the remaining time of the " << nam << " is " << rt->count() << " mins" << std::endl; 
		}
	}
	std::cout << "SiNa" << std::endl;
	return 0;
}
