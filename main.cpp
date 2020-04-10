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
	
	std::cout << "SiNa" << std::endl;
	return 0;
}
