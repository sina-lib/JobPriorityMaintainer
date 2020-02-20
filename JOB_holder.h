
#ifndef _JOB_HOLDER_HEADER_
#define _JOB_HOLDER_HEADER_

class Job
{
public:
	Job();
	~Job(); // default destructor
private:
	char* name;
	int   benefit;
	int   dueDate;
	int   priority;
};

#endif

