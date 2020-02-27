#include <iostream>
#include <chrono>
#include "JOB_holder.h"
#include <iomanip>
#include <chrono>


int main(int argc, char* argv[])
{
	std::cout << "this is a cmake test" << std::endl;
	

	std::time_t nt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	std::cout << std::put_time(std::localtime(&nt),"%c") << std::endl;


	JOB_holder a;
	return 0;
}
