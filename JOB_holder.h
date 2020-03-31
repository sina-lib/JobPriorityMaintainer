#include <chrono>
#include <string>
#include <optional>

#define JobPM_VERSION_MAJOR 0
#define JobPM_VERSION_MINOR 5

class JOB_holder
{
public:
	JOB_holder(); // default constructor
	JOB_holder(const char* addressName); // load this job from an JSON file with the given address
	JOB_holder(const std::string& address);
	JOB_holder(const std::string& name,const std::chrono::system_clock::time_point& deadline, const float reward = 1.0);
	~JOB_holder(); // destructor

	
	std::optional<bool> loadJob(const char* address);
	std::optional<bool> loadJob(const std::string& address);
	bool getRegisteration(void) const;
	bool saveJob(const char* address); // save the current Jon as a JSON into the file address
	std::optional<std::chrono::minutes> getRemainingTime();
	void setDescription(const std::string&);
	void getNameDescription(std::string& name, std::string& description);
	
	
private:
	static size_t count_unknowns;
	std::string name;
	std::string description;
	bool periodic;
	bool isRegistered; // is this object loaded?
	std::chrono::minutes job_repeat_duration;
	std::chrono::system_clock::time_point deadline; // when not periodic, use this as an origin sign
	float reward;

	void updateDeadlineInCaseOfPeriodic(void);
};


/*
  the convention:
  periodic : deadline shall be treated as the next deadline, and can be updated using
             the job_repeat_duration
  non periodic : the deadline is a one-shot deadline.
 */
