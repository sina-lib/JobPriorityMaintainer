
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
	JT_GET_REM,
	JT_QUIT
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
		case 'q':
		case 'Q': return JOB_TST_CMD::JT_QUIT; break;
		default: std::cout << "bad command." << std::endl; return JOB_TST_CMD::JT_DO_NOTHING; break;
		}
	}
}

void doSomeTests()
{
	Validator<std::string> getValidString{std::cin, std::cout};
	Validator<int>         getValidInt   {std::cin, std::cout};
	Validator<char>        getValidChar  {std::cin, std::cout};
	// note : Validator has an operator() function which gives a gentle cout

	// begin doing commands
	bool quitFlag {true};
 	while (quitFlag)
	{
		auto now = std::chrono::system_clock::now();
		// print current time
		std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
		for (size_t i{}; i < 30 ; i++) std::cout << "--";
		std::cout << "It's  >>>  " << std::ctime(&now_time_t);

		
		// list of available commands
		std::cout << "L\tload a job, show deadline" << std::endl;
		std::cout << "C\tcreate  a job" << std::endl;
		std::cout << "Q\tquit the test" << std::endl;
		std::cout << "Enter a command: " << std::endl;

		// get a command
		JOB_TST_CMD cmd = getACmd();

		// do it
		switch ( cmd )
		{

		// =============================================================================
		case JOB_TST_CMD::JT_CREATE:
		{
			JOB_holder* theJob;
			std::cout << "creating a job:" << std::endl;
			std::string name{};
			std::cout << "Enter the name of the job: ";
			getValidString >> name;

			std::string description{};
			std::cout << "Enter a description for the task: ";
			getline(std::cin,description);

			double reward{1.0};
			Validator<double>(std::cin, std::cout)
				("How much will you be rewarded if you complete this job? ") >> reward;
			
			char asker{};
			getValidChar("is this job periodic (Y) or is it just a single deadline(N)?") >> asker;
			if (asker == 'y' || asker == 'Y')
			{
				// JOB is periodic
				int mi{};
				std::chrono::minutes period{};

				std::cout << "making a period for the task. A period is created "
					"by gathering how many days and hours." << std::endl;
				getValidInt("how many days?") >> mi;
				period += std::chrono::hours(24 * mi);

				getValidInt("How many hours? ") >> mi;
				period += std::chrono::hours(mi);

				theJob = new JOB_holder {name, period, reward};
			}
			else // the job is a single shot deadline
			{
				// a single deadline type job
				// get the deadline
				int durations{};
				auto deadLine = getCurrentDayTime();
				std::cout << "Begining from " << getStringFromTimePoint(deadLine) << std::endl;
																					 
				getValidInt("How many days from now on? ") >> durations;
				deadLine += std::chrono::hours(24*durations);

				getValidInt("What hour on that specified day does this due? ") >> durations;
				deadLine += std::chrono::hours(durations);

				// ok, lets create a one deadline job:
				theJob = new JOB_holder {name, deadLine, reward};
			}
			theJob->setDescription(description);
			theJob->prettyPrintThisJob();

			// check to see if the user wants to save it
			getValidChar("Do you want to save this job?") >> asker;
			if (asker == 'y' || asker == 'Y' )
			{
				bool flag{false};
				do
				{
				std::string address{};
				getValidString("Enter a valid address : ") >>  address;
				std::cout << "THE ADDRESS: " << address  << std::endl;
				if (theJob->saveJob(address.data()))
				{
					std::cout << "succesfully saved." << std::endl;
					flag = true;
				}
				else
				{
					std::cout << "could not save it." << std::endl;
				}
				} while (!flag);
			}
			break;
		}

		// ====================================================================================
		case JOB_TST_CMD::JT_LOAD:
		{
			std::cout << "lets load a job" << std::endl;
			std::string add{};
			Validator<std::string> getValidString {std::cin, std::cout };

			// get the address
			getValidString("Enter a valid address: ") >> add;
			
			JOB_holder job{add}; // try to load it

			if (job.getRegisteration())
			{
				std::cout << "Load done." << std::endl;

				job.prettyPrintThisJob();

				std::optional<std::chrono::minutes> x = job.getRemainingTime();
				if (!x)
				{
					std::cout << "there is a problem geting the remaining time" << std::endl;
				}
				else
				{
					int remMins = static_cast<int> (x->count());
					int remDays {remMins / (24*60)};
					int rH {(remMins % (24*60)) / 60};
					std::cout << "Remaining time till next deadline is: " << remMins << " minutes" << std::endl;
																									  std::cout << "Which is " << remDays << " days, " << rH << " hours and "  << "" << std::endl;
				}
			}
			else
			{
				std::cout << "Loading failed." << std::endl;
			}
			
			break;
		}
		// ===================================================================================
		case JOB_TST_CMD::JT_DO_NOTHING: break;
		// ===================================================================================
		case JOB_TST_CMD::JT_QUIT:
		{
			quitFlag = false;
			break;
		}
		default:
		{
			std::cout << "cannot do that" << std::endl;
			break;
		}
		}
	}
}

