#include <chrono>
#include <string>

#define JobPM_VERSION_MAJOR 0
#define JobPM_VERSION_MINOR 5

class JOB_holder
{
public:
	JOB_holder(); // default constructor
	JOB_holder(const std::string& name,const std::chrono::system_clock::time_point& deadline);
	~JOB_holder(); // destructor

	std::chrono::minutes getRemainingTime();
	void setDescription(const std::string&);
	void getNameDescription(std::string& , std::string&);
	
private:
	static size_t count_unknowns;
	std::string name;
	std::string description;
	bool periodic;
	std::chrono::minutes job_repeat_duration;
	std::chrono::system_clock::time_point deadline;
	float reward;
};
