#include "JOB_holder.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <optional>
#include <fstream>
#include <sstream>
#include <chrono>
#include "json.hpp"

using json = nlohmann::json;

size_t JOB_holder::count_unknowns{0};

// Constructors and destructors -----------------------------------------------------------

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
		g >> myJob; // read and deserillize
		this->name = std::string(myJob["name"]);
		this->description = std::string(myJob["description"]);
		this->periodic = myJob["periodic"] == 1 ? true : false;
		this->reward = myJob["reward"];
		std::istringstream ddl{std::string(myJob["deadline"])};
		std::time_t deadL;
		ddl >> deadL;
		this->deadline = std::chrono::system_clock::from_time_t(deadL);
		g.close();
	}
	else
	{
		std::cout << "problem loading a JOB: cannot open the file:" << add << std::endl;
		// TODO: throw an exception !!!
		isRegistered = false;
		this->name = std::string("Unknown job _" + std::to_string(count_unknowns++));
	}
}

JOB_holder::~JOB_holder() // destructor
{
	if (!isRegistered && count_unknowns > 0) count_unknowns--;
	std::cout << "delete" << std::endl ;
}


bool JOB_holder::saveJob(const char* address)
{
	std::ofstream g{address};
	if (g)
	{
		json theJob{};
		theJob["name"] = this->name;
		theJob["description"] = this->description;
		theJob["periodic"] = (this->periodic) ? 1 : 0;
		auto ddl = std::chrono::system_clock::to_time_t(this->deadline);
		std::ostringstream j;
		j << ddl;
		
		theJob["deadline"] = j.str();
		j.clear();
		j << this->job_repeat_duration.count();
		theJob["period"] = j.str() + std::string(" mins");
		g << theJob; // serilize and write it
		g.close();
		return true;
	}
	return false;
}

// GET SETs ---------------------------------------------------------------------------
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

