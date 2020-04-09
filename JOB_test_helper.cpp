
/*
  providing some load, save and time checker/shower here
 */

#include <sstream>

bool loadAJob(JOB_holder* ins)
{
	std::cout << "Enter the path to job: ";
	std::string buf;
	std::cin >> std::ws >> buf;
	
}

enum class JOB_TST_CMD
{
	JT_DO_NOTHING,
	JT_LOAD,
	JT_CREATE,
	JT_GET_REM
};

JOB_TST_CMD getACmd()
{
	while (true)
	{
		char s{};
		std::cin >> s;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); // clear the rest of input
		switch (s)
		{
		case 'L':
		case 'l': return JOB_TST_CMD::JT_LOAD; break;
		case 'c':
		case 'C': return JOB_TST_CMD::JT_CREATE; break;
		default: std::cout << "bad command." << std::endl; return JOB_TST_CMD::JT_DO_NOTHING; break;
		}
	}
}

void doSomeTests()
{
	Validator<std::string> getValidString{std::cin, std::cout};
	Validator<int>         getValidInt   {std::cin, std::cout};
	Validator<char>        getValidChar  {std::cin, std::cout};
	// note : Validator has an operator() function which gives gentle cout

	// begin doing commands
 	while (true)
	{
		auto now = std::chrono::system_clock::now();
		// print current time
		std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
		for (size_t i{}; i < 30 ; i++) std::cout << "--";
		std::cout << "It's  >>>  " << std::ctime(&now_time_t);

		testGetCurrentDayTimeFunction();
		return ;
		//std::cout << "some rounded time : " <<  << std::endl;
		// list of available commands
		std::cout << "L\tload a job, show deadline" << std::endl;
		std::cout << "C\tcreate  a job" << std::endl;
		std::cout << "Enter a command: " << std::endl;

		// get a command
		JOB_TST_CMD cmd = getACmd();

		// do it
		switch ( cmd )
		{

		// =============================================================================
		case JOB_TST_CMD::JT_CREATE:
		{
			std::cout << "creating a job:" << std::endl;
			std::string name{};
			std::cout << "Enter the name of the job: ";
			getValidString >> name;
			char asker{};
			getValidChar("is this job periodic (Y) or is it just a single deadline(N)?") >> asker;
			if (asker == 'y' || asker == 'Y')
			{
				int mi{};
				getValidInt("how many days?") >> mi;
				// TODO
			}
			else
			{
				// a single deadline type job
				// get the deadline
				int durations{};
				getValidInt("How many days from now on? ") >> durations;
				auto deadLine = getCurrentDayTime();
				std::time_t hyperX {std::chrono::system_clock::to_time_t(deadLine)};
				std::cout << "todays time has been rounded: " << std::ctime(&hyperX) << std::endl;
				deadLine += std::chrono::hours(24*durations);
				
			}
			break;
		}

		// ====================================================================================
		case JOB_TST_CMD::JT_LOAD:
		{
			std::cout << "lets load a job" << std::endl;
			std::string add{};
			std::cout << "enter a correct address: "; 
			std::cin >> add;
			//JOB_holder job{add};
			break;
		}
		case JOB_TST_CMD::JT_DO_NOTHING: break;
		default:
		{
			std::cout << "cannot do that" << std::endl;
			break;
		}
		}
	}
}

