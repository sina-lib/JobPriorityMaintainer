#include "JOB_holder.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <optional>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

size_t JOB_holder::count_unknowns{0};

JOB_holder::JOB_holder() : periodic{false} , isRegistered{false} // default constructor
{
	std::cout << "default JOB constructor" << std::endl;
	this->name = "Unknown job _" + std::to_string(count_unknowns++);
}

JOB_holder::JOB_holder(const char* add) : isRegistered{true}
{
	std::ifstream g{add};
	if (g)
	{
		std::string content{};
		/*std::string a{};
		while (!g.eof())
		{
			if (g >> a)
				content += a;
				}*/
		json myJob{};
		g >> myJob;
		std::cout << myJob["name"] << std::endl;
		// got to deseriallize it
		g.close();
	}
	else
	{
		std::cout << "problem loading a JOB: cannot open the file:" << add << std::endl;
		// TODO: throw an exception !!!
	}
}

JOB_holder::~JOB_holder() // destructor
{
	if (!isRegistered && count_unknowns > 0) count_unknowns--;
	std::cout << "delete" << std::endl ;
}



void JOB_holder::setDescription(const std::string& desc)
{
	this->description = desc;
}

void JOB_holder::getNameDescription(std::string& nam, std::string& desc)
{
	nam = this->name;
	desc = this->description;
}

std::optional<std::chrono::minutes> JOB_holder::getRemainingTime()
{
	if (!isRegistered) return std::nullopt;
	
	auto n = std::chrono::system_clock::now();

	if (periodic)
	{
	}
	else
	{
	}

	return std::nullopt;
}

