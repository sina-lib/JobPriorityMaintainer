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

// neither load nor create - Bare JOB -----------------------------------------------------
JOB_holder::JOB_holder() : periodic{false} , isRegistered{false} // default constructor
{
	std::cout << "default JOB constructor" << std::endl;
	this->name = "Unknown job _" + std::to_string(count_unknowns++);
}

// ===================================== LOADERS =====================================
// load a JOB from given address ---------------------------------------------------------
JOB_holder::JOB_holder(const std::string& add) : isRegistered{true} // read from file
{
	std::ifstream g{add};
	bool doUpdate{false};
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
		ddl >> deadL; // should read the deadline as an epoch on seconds basis (extracts one long int from the string)
		this->deadline = std::chrono::system_clock::from_time_t(deadL); // change it to time_point
		ddl.str(myJob["period"]);
		ddl.clear(); // clear the error bits
		long int per_mins{};
		ddl >> per_mins;
		this->job_repeat_duration = std::chrono::minutes(per_mins);

		// check to see if the file must be updated
		if (periodic) // in case of a periodic, the deadline should always be tuned to the next
		{
			auto rem = this->getRemainingTime();
			if (rem->count() < 0 )
			{
				std::cout << "Warning: this is periodic JOB and its' next deadline is out of date." << std::endl;
				doUpdate = true;
			}
		}
		//}
		g.close();
		if (doUpdate)
		{
			std::cout << "Going to update the JOB's deadline given the period..." << std::endl;
			// first, correct the next deadline;
			this->updateDeadlineInCaseOfPeriodic();
			
			// we have the address
			if (this->saveJob(add.data()))
				std::cout << "the " << name << " JOB's next deadline updated and saved new one." << std::endl;
			else
				std::cout << "there is problem saving a JOB (" << name << ")"  << std::endl;
		}
	}
	else
	{
		std::cout << "problem loading a JOB: cannot open the file:" << add << std::endl;
		// TODO: throw an exception !!!
		isRegistered = false;
		this->name = std::string("Unknown_job_" + std::to_string(count_unknowns++));
	}
}

// load a job from file ---------------------------------------------------------------------------
JOB_holder::JOB_holder (const char* address) : JOB_holder(std::string(address))
{
}

// ==================================== CREATORS =========================================
// One shot deadline JOB constructor ---------------------------------------------------------------
JOB_holder::JOB_holder(const std::string& Name, const std::chrono::system_clock::time_point& deadLine, const double rew): name{Name} , deadline{deadLine} , description{""} , periodic{false} , isRegistered{true} , reward{rew}
{
	std::time_t dead {std::chrono::system_clock::to_time_t(deadLine)};
	std::cout << "created a Job with name: " << Name << " & deadline: " << std::ctime(&dead) << std::endl;
}

// Periodic JOB constructor -----------------------------------------------------------
JOB_holder::JOB_holder(const std::string& name_, const std::chrono::minutes& repDuration, const double rew ) : periodic{true} , isRegistered{true} , name{name_} , reward{rew} ,
	    job_repeat_duration{repDuration}
{
	// TODO : check if it is a correct one
	
	std::cout << "Created a Periodic JOB with name " << name << std::endl;
	int mins {static_cast<int> (repDuration.count())};
	int days {mins / (24*60)};
	int hours {mins % (24*60)};
	hours /= (60);
	mins = mins % 60;
	std::cout << "Duration : " << days << " days and " << hours << " hours and " << mins << " minutes." << std::endl;
	this->deadline = std::chrono::system_clock::now();
	updateDeadlineInCaseOfPeriodic();
}

// Destruct a JOB --------------------------------------------------------------------------------
JOB_holder::~JOB_holder() // destructor
{
	// take care of trivial JOBs
	if (!isRegistered && count_unknowns > 0) count_unknowns--;
	// warn the user
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

// -----------------------------------------------------------------------------------------
// Remaining time for the next deadline of this job (wether it is a period one or one-shot)
std::optional<std::chrono::minutes> JOB_holder::getRemainingTime()
{
	if (!isRegistered) return std::nullopt; // to avoid calling on an un-initiallized JOB
	
	auto now_time = std::chrono::system_clock::now();
	std::chrono::minutes remTime{0};
	
	
	if (periodic)
		this->updateDeadlineInCaseOfPeriodic();
	
	remTime = std::chrono::duration_cast<std::chrono::minutes>( this->deadline - now_time );

	return remTime;
}


// ---------------------------------------------------------------------------------
// this is a private method , it is internally used
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


// print all the information of this job -----------------------------------------
void JOB_holder::prettyPrintThisJob(void)
{
	
	using std::cout;
	using std::endl;
	if (isRegistered)
	{
		for (size_t i{}; i<25; i++) cout << "==" ;
		cout << endl;
		cout << "Details of a " << ((periodic) ? "periodic " : "one-shot ") << " JOB is as following:"  << endl;
		cout << "name:        " << this->name << endl;
		cout << "description: " << description << endl;
		cout << "reward after completion: " << this->reward << endl;
		if (periodic)
		{
			// show the period
			cout << "Period : " << job_repeat_duration.count() << " minutes, ";
			auto inHours= std::chrono::duration_cast<std::chrono::hours>(job_repeat_duration).count();
		    cout << "which is " << (inHours / 24) << " days and " << (inHours % 24) << " hours." << endl; 
			// show the remainig time until next due
		}
		else
		{
			// show the deadline, and remaining time
			std::optional<std::chrono::minutes> rem = getRemainingTime();
			if (!rem) cout << "what the fuck? the job is registered, but cann't get the remaining time!" << endl;
			int x = rem->count();
			int days {x / (24*60)};
			cout << "remaining time in minutes: " << x << endl;
			cout << "remaining time of this job is " << days << " days and " << (x % (24*60)) / 60 << "hours and " << (x % 60) << " minutes." << endl;
		}
	}
	else
	{
		cout << "warning: this job is not registered!" << endl;
	}
}
