#include "utils.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <string>


my_exception::my_exception(const std::string &arg, const char *file, int line) : std::runtime_error(arg) 
{
	msg = std::string(file) + ":" + std::to_string(line) + ": " + arg;
}

my_exception::~my_exception() throw(){}

const char* my_exception::what() const throw() {
	return msg.c_str();
}