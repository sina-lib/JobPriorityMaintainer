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

JOB_holder::JOB_holder(const std::string& add) : isRegistered{true} // read from file
{
	std::ifstream g{add};
	if (g)
	{
		/*std::string content{};
		std::string a{};
		while (!g.eof())
		{
			if (g >> a)
				content += a;
				}*/
		//try {
		json myJob{};
		g >> myJob; // read and deserillize
		this->name = std::string(myJob["name"]);
		this->description = std::string(myJob["description"]);
		this->periodic = myJob["periodic"] == true;
		this->reward = myJob["reward"];
		std::istringstream ddl{std::string(myJob["deadline"])};
		std::time_t deadL;
		ddl >> deadL; // should read the deadline as an epoch on seconds basis
		this->deadline = std::chrono::system_clock::from_time_t(deadL); // change it to time_point
		ddl.str(myJob["period"]);
		ddl.clear();
		long int per_mins{};
		ddl >> per_mins;
		this->job_repeat_duration = std::chrono::minutes(per_mins);
		//}
		g.close();
	}
	else
	{
		std::cout << "problem loading a JOB: cannot open the file:" << add << std::endl;
		// TODO: throw an exception !!!
		isRegistered = false;
		this->name = std::string("Unknown_job_" + std::to_string(count_unknowns++));
	}
}

JOB_holder::JOB_holder (const char* address) : JOB_holder(std::string(address))
{
}

JOB_holder::JOB_holder(const std::string& Name, const std::chrono::system_clock::time_point& deadLine, const float rew): name{Name} , deadline{deadLine} , description{""} , periodic{false} , isRegistered{true} , reward{rew}
{
	std::time_t dead {std::chrono::system_clock::to_time_t(deadLine)};
	std::cout << "created a Job with name: " << Name << " & deadline: " << std::ctime(&dead) << std::endl;
}

JOB_holder::~JOB_holder() // destructor
{
	if (!isRegistered && count_unknowns > 0) count_unknowns--;
	std::cout << "a JOB holder (" << name << ") deleted." << std::endl ;
}


// ================================================================================
bool JOB_holder::saveJob(const char* address)
{
	std::ofstream g{address};
	if (g)
	{
		json theJob{};
		theJob["name"] = this->name;
		theJob["description"] = this->description;
		theJob["periodic"] = this->periodic;

		// insert the deadline in a pretty way
		auto ddl = std::chrono::system_clock::to_time_t(this->deadline);
		std::ostringstream j;
		j << ddl; // this should create an epoch integer, on a second basis
		theJob["deadline"] = j.str();

		// insert repeat duration as of minustes
		j.str("");
		j.clear();
		j << this->job_repeat_duration.count() << " mins";//repeat is a simple long expressing minutes
		theJob["period"] = j.str();
        //theJob["period"] = std::to_string(this->job_repeat_duration.count()) + std::string(" mins");
												  
		theJob["reward"] = this->reward;
		g << theJob; // serilize and write it
		g.close();
		return true;
	}
	return false;
}


std::optional<bool> JOB_holder::loadJob(const std::string& address)
{
	std::ifstream g{address};
	if (g)
	{
		//try {
		json myJob{};
		g >> myJob; // read and deserillize
		this->name = std::string(myJob["name"]);
		this->description = std::string(myJob["description"]);
		this->periodic = myJob["periodic"] == true;
		this->reward = myJob["reward"];

		std::istringstream ddl{std::string(myJob["deadline"])};
		std::time_t deadL;
		ddl >> deadL; // should read the deadline as an epoch on seconds basis
		this->deadline = std::chrono::system_clock::from_time_t(deadL); // change it to time_point

		ddl.str(std::string(myJob["period"]));
		ddl.clear();
		long int mins;
		ddl >> mins;
		this->job_repeat_duration = std::chrono::minutes(mins);
		//}
		g.close();
		this->isRegistered = true;
		return true;
	}
	else
	{
		std::cout << "problem loading a JOB: cannot open the file:" << address << std::endl;
		// TODO: throw an exception !!!
		isRegistered = false;
		this->name = std::string("Unknown_job_" + std::to_string(count_unknowns++));
		return false;
	}
	return std::nullopt;
}

std::optional<bool> JOB_holder::loadJob(const char* add)
{
	return this->loadJob(std::string(add));
}
// GET SETs ---------------------------------------------------------------------------
bool JOB_holder::getRegisteration(void) const { return this->isRegistered; };

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
	
	auto now_time = std::chrono::system_clock::now();
	std::chrono::minutes remTime{0};
	
	
	if (periodic)
		this->updateDeadlineInCaseOfPeriodic();
	
	remTime = std::chrono::duration_cast<std::chrono::minutes>( this->deadline - now_time );

	return remTime;
}

void JOB_holder::updateDeadlineInCaseOfPeriodic(void)
{
	auto now_time = std::chrono::system_clock::now();
	auto interv = std::chrono::duration_cast<std::chrono::minutes> (this->deadline - now_time);
	
	while (interv.count() < 0)
	{
		// we have passed last deadline, adding Period untill this becomes positive
		this->deadline += this->job_repeat_duration;
		interv = std::chrono::duration_cast<std::chrono::minutes> (this->deadline - now_time);
	}
}

