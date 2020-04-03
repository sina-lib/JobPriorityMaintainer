
/*
  providing some load, save and time checker/shower here
 */

bool loadAJob(JOB_holder* ins)
{
	std::cout << "Enter the path to job: ";
	std::string buf;
	std::cin >> std::ws >> buf;
	
}

enum class JOB_TST_CMD
{
	JT_LOAD,
	JT_CREATE,
	JT_GET_REM
};

JOB_TST_CMD getACmd()
{
}

void doSomeTests()
{
	while (true)
	{
		auto now = std::chrono::system_clock::now();
		std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
		std::cout << "It's " << std::ctime(&now_time_t) << std::endl;

		std::cout << "Enter a command: " << std::endl;
		JOB_TST_CMD cmd = getACmd();
		switch (cmd )
		{
		case JOB_TST_CMD::JT_CREATE: break;
		default: std::cout << "" << std::endl;
		}
	}
}

