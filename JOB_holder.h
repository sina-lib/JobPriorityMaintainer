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
	JOB_holder(const std::string& name,const std::chrono::system_clock::time_point& deadline);
	~JOB_holder(); // destructor
	
	
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
	std::chrono::system_clock::time_point deadline;
	float reward;
};
