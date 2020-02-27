#include "JOB_holder.h"
#include <iostream>
#include <iomanip>

static size_t JOB_holder::count_unknowns{0};

JOB_holder::JOB_holder() : periodic{false} // default constructor
{
	std::cout << "default JOB constructor" << std::endl;
	this->name = "Unknown job _" + std::string(count_unknowns++);
}


JOB_holder::~JOB_holder() // destructor
{
	std::cout << 
}
