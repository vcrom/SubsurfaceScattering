#include "exceptions.h"

my_exception::my_exception(const std::string &arg, const char *file, int line) : std::runtime_error(arg)
{
	msg = std::string(file) + ":" + std::to_string(line) + ": " + arg;
}

my_exception::~my_exception() throw()
{
	msg.empty();
}

const char* my_exception::what() const throw() {
	return msg.c_str();
}


critical_exception::critical_exception(const std::string &arg, const char *file, int line) 
	: my_exception(arg, file, line)
{}

non_critical_exception::non_critical_exception(const std::string &arg, const char *file, int line)
	: my_exception(arg, file, line)
{}
