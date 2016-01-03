#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class my_exception : public std::runtime_error {
public:
	my_exception(const std::string &arg, const char *file, int line);
	~my_exception() throw();
	const char *what() const throw();
private:
	std::string msg;
};


class critical_exception : public my_exception {
public:
	critical_exception(const std::string &arg, const char *file, int line);
	//~critical_exception() throw();
};

class non_critical_exception : public my_exception {
public:
	non_critical_exception(const std::string &arg, const char *file, int line);
};

#define throw_non_critical(arg) throw non_critical_exception(arg, __FILE__, __LINE__);
#define throw_critical(arg) throw critical_exception(arg, __FILE__, __LINE__);

#endif // EXCEPTIONS_H
