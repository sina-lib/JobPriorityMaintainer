#include <iostream>
#include <chrono>
#include "JOB_holder.h"
#include <iomanip>
#include <chrono>
#include "Validator.hpp"
#include "TimeUtility.h"
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include <sstream>

inline void print(const std::string& matn)
{
	std::cout << matn << std::endl;
}
#include "JOB_test_helper.cpp"

using namespace boost::filesystem;

int main(int argc, char* argv[])
{

	// print a simple size
	std::cout << "size of JPM exec is " << file_size("./JPM") << " bytes" << std::endl;

	// goto a folder
	path p("rjobs");
	if (exists(p))
	{
		if (is_regular_file(p))
			std::cout << "p is a file!" << std::endl;
		else
		{
			std::cout << "contents of p: " << std::endl;
			for (directory_entry& x : directory_iterator(p))
			{
				std::cout  << "    " << x.path() << '\n';

				std::ostringstream name{};
				name << x.path();

				std::string p  = name.str();
				p.erase(p.begin());
				p.erase(p.end()-1);
				
				JOB_holder a{p};
				a.prettyPrintThisJob();
			}
		}
	}
	
	
	std::cout << "SiNa" << std::endl;
	return 0;
}
